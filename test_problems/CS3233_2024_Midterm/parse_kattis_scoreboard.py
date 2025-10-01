import requests
from bs4 import BeautifulSoup
import pandas as pd
import re

def parse_kattis_standings(url):
    """
    Parse Kattis contest standings from the given URL
    
    Args:
        url (str): URL of the Kattis standings page
    
    Returns:
        dict: Parsed standings data with teams and problems
    """
    
    # Fetch the HTML content
    response = requests.get(url)
    response.raise_for_status()
    
    soup = BeautifulSoup(response.content, 'html.parser')
    
    # Find the standings table
    standings_table = soup.find('table', class_=['table2', 'sticky-header', 'standings-table'])
    if not standings_table:
        # Try alternative selectors
        standings_table = soup.find('table')
        if not standings_table:
            raise ValueError("Could not find standings table")
    
    print(f"Found table with classes: {standings_table.get('class', [])}")
    
    # Parse problem headers from table header
    problems = []
    header_row = standings_table.find('thead')
    if header_row:
        header_cells = header_row.find_all('th')
        print(f"Found {len(header_cells)} header cells")
        
        for i, cell in enumerate(header_cells):
            cell_text = cell.get_text(strip=True)
            data_name = cell.get('data-name', '')
            
            print(f"  Header {i}: text='{cell_text}', data-name='{data_name}'")
            
            # Problem columns usually have data-name starting with 'problem'
            if data_name.startswith('problem'):
                problem_id = data_name.replace('problem', '').upper()
                if problem_id:
                    problems.append(problem_id)
                else:
                    problems.append(cell_text)
            elif cell_text and cell_text not in ['GROUP', 'SLV.', 'TIME', 'RANK', 'TEAM', 'SOLVED', 'PENALTY']:
                # If it's a single letter or looks like a problem identifier
                if len(cell_text) <= 2 and cell_text.isalpha():
                    problems.append(cell_text.upper())
    
    print(f"Identified problems: {problems}")
    
    # Parse team data from table body
    teams_data = []
    
    # Find all team rows in tbody
    tbody = standings_table.find('tbody')
    if tbody:
        team_rows = tbody.find_all('tr')
    else:
        # Fallback: get all rows except header
        all_rows = standings_table.find_all('tr')
        team_rows = all_rows[1:] if len(all_rows) > 1 else all_rows
    
    print(f"Found {len(team_rows)} team rows to process")
    
    for row_idx, row in enumerate(team_rows[:-1]):
        cells = row.find_all(['td', 'th'])
        if len(cells) < 3:  # Skip rows with too few cells
            continue
            
        print(f"\nProcessing team row {row_idx + 1} with {len(cells)} cells")
        
        team_data = {}
        cell_idx = 0
        
        # Parse rank (usually first cell)
        if cell_idx < len(cells):
            rank_text = cells[cell_idx].get_text(strip=True)
            team_data['rank'] = rank_text
            cell_idx += 1
            print(f"  Rank: '{rank_text}'")
        
        # Parse team name (next cell)
        if cell_idx < len(cells):
            team_cell = cells[cell_idx]
            team_name = team_cell.get_text(strip=True)
            
            # Extract just the team name part (before parentheses if present)
            if '(' in team_name:
                team_name = team_name.split('(')[0].strip()
            
            team_data['team_name'] = team_name
            cell_idx += 1
            print(f"  Team: '{team_name}'")
        
        # Parse solved count and time penalty (next 2 cells typically)
        if cell_idx < len(cells):
            solved_text = cells[cell_idx].get_text(strip=True)
            team_data['problems_solved'] = solved_text
            cell_idx += 1
            print(f"  Solved: '{solved_text}'")
        
        if cell_idx < len(cells):
            time_text = cells[cell_idx].get_text(strip=True)
            team_data['total_time'] = time_text
            cell_idx += 1
            print(f"  Time: '{time_text}'")
        
        # Parse individual problem results
        team_data['problems'] = {}
        
        for prob_idx, prob_name in enumerate(problems):
            if cell_idx < len(cells):
                prob_cell = cells[cell_idx]
                
                # Initialize problem info
                problem_info = {
                    'status': 'unattempted',
                    'attempts': 0,
                    'time': None,
                    'text': '',
                    'classes': prob_cell.get('class', [])
                }
                
                # Look for problem result indicators
                cell_classes = prob_cell.get('class', [])
                
                # Check for solved indicator (usually green checkmark or "solved" class)
                if any('solved' in cls.lower() for cls in cell_classes):
                    problem_info['status'] = 'accepted'
                    
                    # Look for time and attempts in the cell
                    spans = prob_cell.find_all('span')
                    for span in spans:
                        span_text = span.get_text(strip=True)
                        span_class = span.get('class', [])
                        
                        if 'standings-table-result-cell' in span_class:
                            problem_info['text'] = span_text
                            # Parse attempts and time from text like "1\n65 min"
                            parts = span_text.split('\n')
                            if len(parts) >= 2:
                                try:
                                    problem_info['attempts'] = int(parts[0])
                                    time_part = parts[1].replace('min', '').strip()
                                    problem_info['time'] = int(time_part)
                                except (ValueError, IndexError):
                                    pass
                
                # Check for failed attempts (usually red X or error class)
                elif any('error' in cls.lower() or 'wrong' in cls.lower() or 'incorrect' in cls.lower() for cls in cell_classes):
                    problem_info['status'] = 'wrong_answer'
                    
                    # Extract attempt count
                    cell_text = prob_cell.get_text(strip=True)
                    if cell_text.isdigit():
                        problem_info['attempts'] = int(cell_text)
                    problem_info['text'] = cell_text
                
                else:
                    # Check cell content for any indicators
                    cell_text = prob_cell.get_text(strip=True)
                    problem_info['text'] = cell_text
                    
                    if cell_text:
                        # Look for patterns that indicate attempts or results
                        if 'min' in cell_text or '/' in cell_text:
                            problem_info['status'] = 'accepted'
                            # Try to parse time
                            time_match = re.search(r'(\d+)\s*min', cell_text)
                            if time_match:
                                problem_info['time'] = int(time_match.group(1))
                        elif cell_text.isdigit():
                            # Could be attempts
                            problem_info['status'] = 'wrong_answer'
                            problem_info['attempts'] = int(cell_text)
                
                team_data['problems'][prob_name] = problem_info
                print(f"    {prob_name}: {problem_info['text']} -> {problem_info['status']}")
                cell_idx += 1
            else:
                # No more cells, mark as unattempted
                team_data['problems'][prob_name] = {
                    'status': 'unattempted',
                    'attempts': 0,
                    'time': None,
                    'text': '',
                    'classes': []
                }
        
        teams_data.append(team_data)
    
    # Extract contest information
    contest_info = {}
    
    # Try to get contest title from page title or headers
    title_elem = soup.find('title')
    if title_elem:
        contest_info['page_title'] = title_elem.get_text(strip=True)
    
    # Look for contest name in the page
    for header_tag in ['h1', 'h2', 'h3']:
        header = soup.find(header_tag)
        if header:
            contest_info['contest_title'] = header.get_text(strip=True)
            break
    
    return {
        'contest_info': contest_info,
        'problems': problems,
        'teams': teams_data,
        'total_teams': len(teams_data)
    }

def create_kattis_summary_dataframe(data):
    """
    Create a summary DataFrame for Kattis standings
    
    Args:
        data (dict): Parsed standings data
    
    Returns:
        pd.DataFrame: Summary standings data
    """
    
    rows = []
    
    for team in data['teams']:
        row = {
            'Rank': team.get('rank', ''),
            'Team_Name': team.get('team_name', ''),
            'Problems_Solved': team.get('problems_solved', ''),
            'Total_Time': team.get('total_time', ''),
        }
        
        # Add problem results
        for prob_name in data['problems']:
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                if prob_data['status'] == 'accepted':
                    attempts = prob_data.get('attempts', 1)
                    time = prob_data.get('time', '')
                    if attempts and time:
                        row[f'Problem_{prob_name}'] = f"{attempts}/{time}"
                    else:
                        row[f'Problem_{prob_name}'] = prob_data.get('text', '+')
                elif prob_data['status'] == 'wrong_answer':
                    attempts = prob_data.get('attempts', 1)
                    row[f'Problem_{prob_name}'] = f"-{attempts}" if attempts > 0 else prob_data.get('text', '-')
                else:
                    row[f'Problem_{prob_name}'] = ''
            else:
                row[f'Problem_{prob_name}'] = ''
        
        rows.append(row)
    
    return pd.DataFrame(rows)

def calculate_kattis_problem_statistics(data):
    """
    Calculate problem statistics for Kattis contest
    
    Args:
        data (dict): Parsed standings data
    
    Returns:
        pd.DataFrame: Problem statistics
    """
    
    problem_stats = []
    total_teams = len(data['teams'])
    
    for prob_name in data['problems']:
        teams_solved = 0
        teams_wrong = 0
        teams_unattempted = 0
        total_attempts = 0
        solve_times = []
        
        for team in data['teams']:
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                if prob_data['status'] == 'accepted':
                    teams_solved += 1
                    attempts = prob_data.get('attempts', 1)
                    if attempts:
                        total_attempts += attempts
                    time = prob_data.get('time')
                    if time:
                        solve_times.append(time)
                        
                elif prob_data['status'] == 'wrong_answer':
                    teams_wrong += 1
                    attempts = prob_data.get('attempts', 1)
                    if attempts:
                        total_attempts += attempts
                else:
                    teams_unattempted += 1
            else:
                teams_unattempted += 1
        
        teams_tried = teams_solved + teams_wrong
        solve_percentage = (teams_solved / total_teams * 100) if total_teams > 0 else 0
        first_solve_time = min(solve_times) if solve_times else 0
        avg_solve_time = sum(solve_times) / len(solve_times) if solve_times else 0
        
        stats = {
            'Problem': prob_name,
            'Teams_Solved': teams_solved,
            'Teams_Wrong': teams_wrong,
            'Teams_Unattempted': teams_unattempted,
            'Teams_Tried': teams_tried,
            'Solve_Percentage': round(solve_percentage, 1),
            'Total_Attempts': total_attempts,
            'First_Solve_Time': first_solve_time,
            'Average_Solve_Time': round(avg_solve_time, 1)
        }
        
        problem_stats.append(stats)
    
    return pd.DataFrame(problem_stats)

def save_kattis_data_to_csv(data, filename_prefix="kattis_contest"):
    """
    Save Kattis standings data to CSV files
    
    Args:
        data (dict): Parsed standings data
        filename_prefix (str): Prefix for output files
    
    Returns:
        dict: Dictionary with filenames of created files
    """
    
    files_created = {}
    
    # 1. Summary standings
    summary_df = create_kattis_summary_dataframe(data)
    summary_filename = f"{filename_prefix}_standings.csv"
    summary_df.to_csv(summary_filename, index=False)
    files_created['standings'] = summary_filename
    print(f"✓ Saved standings to: {summary_filename}")
    
    # 2. Problem statistics
    stats_df = calculate_kattis_problem_statistics(data)
    stats_filename = f"{filename_prefix}_problem_stats.csv"
    stats_df.to_csv(stats_filename, index=False)
    files_created['problem_stats'] = stats_filename
    print(f"✓ Saved problem statistics to: {stats_filename}")
    
    return files_created

def display_kattis_summary(data):
    """
    Display a summary of the parsed Kattis data
    
    Args:
        data (dict): Parsed standings data
    """
    
    print("\n" + "="*60)
    print("KATTIS CONTEST SUMMARY")
    print("="*60)
    print(f"Contest: {data['contest_info'].get('contest_title', 'Unknown')}")
    print(f"Total teams: {data['total_teams']}")
    print(f"Total problems: {len(data['problems'])}")
    print(f"Problems: {', '.join(data['problems'])}")
    
    print(f"\nTop 10 teams:")
    print("-" * 60)
    for i, team in enumerate(data['teams'][:10]):
        solved_count = team.get('problems_solved', '?')
        total_time = team.get('total_time', '?')
        print(f"{team.get('rank', '?'):>3}. {team.get('team_name', 'Unknown'):<30} | Solved: {solved_count:>2} | Time: {total_time}")

# Example usage
if __name__ == "__main__":
    url = "https://nus.kattis.com/courses/CS3233/CS3233_S2_AY2324/assignments/xbuv2a/standings"
    
    try:
        print("Parsing Kattis standings...")
        standings_data = parse_kattis_standings(url)
        
        display_kattis_summary(standings_data)
        
        # Show problem statistics
        print("\n" + "="*60)
        print("PROBLEM STATISTICS")
        print("="*60)
        problem_stats = calculate_kattis_problem_statistics(standings_data)
        print(problem_stats.to_string(index=False))
        
        print(f"\nSaving data to CSV files...")
        files_created = save_kattis_data_to_csv(standings_data, "cs3233_contest")
        
        print(f"\n📁 Files created:")
        for file_type, filename in files_created.items():
            print(f"  {file_type.title()}: {filename}")
        
        print(f"\n🎉 Successfully parsed {standings_data['total_teams']} teams and {len(standings_data['problems'])} problems!")
        
    except Exception as e:
        print(f"❌ Error: {e}")
        import traceback
        traceback.print_exc()