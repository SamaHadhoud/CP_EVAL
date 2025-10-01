import requests
from bs4 import BeautifulSoup
import pandas as pd

def debug_table_structure(url):
    """
    Debug function to analyze the exact table structure
    """
    response = requests.get(url)
    response.raise_for_status()
    soup = BeautifulSoup(response.content, 'html.parser')
    
    scoreboard_table = soup.find('table', class_='scoreboard')
    if not scoreboard_table:
        print("No scoreboard table found!")
        return
    
    print("=== TABLE STRUCTURE DEBUG ===")
    
    # Analyze all rows
    all_rows = scoreboard_table.find_all('tr')
    print(f"Total rows found: {len(all_rows)}")
    
    for i, row in enumerate(all_rows[:5]):  # First 5 rows
        print(f"\nRow {i}:")
        row_class = row.get('class', [])
        print(f"  Classes: {row_class}")
        
        cells = row.find_all(['td', 'th'])
        print(f"  Cells: {len(cells)}")
        
        for j, cell in enumerate(cells[:10]):  # First 10 cells
            cell_text = cell.get_text(strip=True)
            cell_class = cell.get('class', [])
            cell_colspan = cell.get('colspan', '1')
            cell_rowspan = cell.get('rowspan', '1')
            
            print(f"    Cell {j}: text='{cell_text}' class={cell_class} colspan={cell_colspan} rowspan={cell_rowspan}")

def parse_icpc_scoreboard(url):
    """
    Parse ICPC scoreboard from the given URL
    
    Args:
        url (str): URL of the scoreboard
    
    Returns:
        dict: Parsed scoreboard data with teams and problems
    """
    
    # Fetch the HTML content
    response = requests.get(url)
    response.raise_for_status()
    soup = BeautifulSoup(response.content, 'html.parser')
    
    # Find the main scoreboard table
    scoreboard_table = soup.find('table', class_='scoreboard')
    if not scoreboard_table:
        raise ValueError("Could not find scoreboard table")
    
    # Parse all column headers from all colgroups
    all_column_ids = []
    problem_columns = []
    
    # Get all colgroup elements
    colgroups = scoreboard_table.find_all('colgroup')
    print(f"Found {len(colgroups)} colgroup(s)")
    
    for i, colgroup in enumerate(colgroups):
        print(f"\nColgroup {i+1}:")
        cols = colgroup.find_all('col')
        
        for col in cols:
            col_id = col.get('id', 'NO_ID')
            col_class = col.get('class', [])
            all_column_ids.append(col_id)
            
            print(f"  Column: id='{col_id}', class='{col_class}'")
            
            # Identify problem columns
            if 'scoreprob' in col_class or (col_id and col_id not in ['scorerank', 'scoreaffil', 'scoreteamname', 'scoresolv', 'scoretotal']):
                if col_id and col_id != 'NO_ID':
                    problem_columns.append(col_id)
    
    print(f"\nAll column IDs: {all_column_ids}")
    print(f"Problem columns: {problem_columns}")
    
    # Extract problem names/letters from the header row
    header_row = scoreboard_table.find('tr', class_='scoreheader')
    problem_names = []
    
    if header_row:
        header_cells = header_row.find_all('th')
        print(f"\nFound {len(header_cells)} header cells")
        
        for i, cell in enumerate(header_cells):
            cell_text = cell.get_text(strip=True)
            cell_scope = cell.get('scope', '')
            cell_title = cell.get('title', '')
            
            print(f"  Header {i}: text='{cell_text}', scope='{cell_scope}', title='{cell_title}'")
            
            # Extract problem names from header cells with scope="col"
            # Skip non-problem columns
            if (cell_scope == 'col' and cell_text and 
                cell_text not in ['rank', 'team name', '# solved / penalty time', 'score', 'team'] and
                not cell_text.lower().startswith('team')):
                problem_names.append(cell_text)
    
    print(f"Problem names from headers: {problem_names}")
    
    # Create problem mapping - filter out non-problem columns
    problems = []
    if problem_names:
        problems = problem_names
    elif problem_columns:
        # Fallback to column IDs, clean them up
        for col_id in problem_columns:
            # Skip team-related columns
            if col_id.lower() in ['scoreteam', 'team', 'scoreteamname']:
                continue
                
            if col_id.startswith('score'):
                # Remove 'score' prefix and get the problem identifier
                prob_id = col_id[5:].upper()  # Remove 'score' (5 chars)
                if prob_id and prob_id not in ['TEAM', 'TEAMNAME', 'AFFIL']:
                    problems.append(prob_id)
            else:
                if col_id.upper() not in ['TEAM', 'TEAMNAME', 'AFFIL']:
                    problems.append(col_id)
    
    print(f"Final problems list: {problems}")
    
    # Parse team data
    teams_data = []
    team_rows = scoreboard_table.find_all('tr')
    
    # Find actual data rows (skip header)
    data_rows = []
    for row in team_rows:
        row_class = row.get('class', [])
        # Skip header rows
        if 'scoreheader' not in row_class:
            cells = row.find_all(['td', 'th'])
            if len(cells) >= len(problems) + 3:  # rank + team + problems + score columns
                data_rows.append(row)
    
    print(f"Found {len(data_rows)} data rows to process")
    
    for row_idx, row in enumerate(data_rows[:-1]):
        cells = row.find_all(['td', 'th'])
        print(f"\nProcessing team row {row_idx + 1} with {len(cells)} cells")
        
        # Debug: print all cell contents
        for i, cell in enumerate(cells[:5]):  # First 5 cells for debugging
            print(f"  Cell {i}: '{cell.get_text(strip=True)}' classes: {cell.get('class', [])}")
        
        team_data = {}
        cell_idx = 0
        
        # Rank (first cell)
        rank_cell = cells[cell_idx]
        team_data['rank'] = rank_cell.get_text(strip=True)
        cell_idx += 1
        print(f"  Rank: '{team_data['rank']}'")
        
        # Team name - need to handle complex structure
        # Look for team name which might be split across cells or have nested elements
        team_name = ""
        team_affiliation = ""
        
        # Check if there's a single cell with team info or multiple cells
        for i in range(cell_idx, min(cell_idx + 4, len(cells))):
            cell = cells[i]
            cell_classes = cell.get('class', [])
            
            # Skip cells that are clearly problems (have score classes)
            if any('score_' in cls for cls in cell_classes):
                break
            
            # Look for team name in the cell content
            cell_html = str(cell)
            cell_text = cell.get_text(strip=True)
            
            # Check if this cell contains team information
            if ('team' in cell_html.lower() or 
                len(cell_text) > 5 or  # Likely contains meaningful text
                any(tag in cell_html.lower() for tag in ['img', 'flag', 'university'])):
                
                # Extract team name and affiliation separately
                # Look for nested elements that might separate team name from affiliation
                team_elements = cell.find_all(text=True, recursive=True)
                text_parts = [text.strip() for text in team_elements if text.strip()]
                
                if len(text_parts) >= 2:
                    # Assume first significant text is team name, rest is affiliation
                    team_name = text_parts[0]
                    team_affiliation = ' '.join(text_parts[1:])
                elif len(text_parts) == 1:
                    team_name = text_parts[0]
                elif cell_text:
                    team_name = cell_text
                
                cell_idx = i + 1
                break
        
        # If we still don't have a good team name, try a different approach
        if not team_name or len(team_name) < 3:
            # Look for the largest text content in the next few cells
            best_text = ""
            for i in range(cell_idx, min(cell_idx + 3, len(cells))):
                cell_text = cells[i].get_text(strip=True)
                cell_classes = cells[i].get('class', [])
                
                if (not any('score_' in cls for cls in cell_classes) and 
                    len(cell_text) > len(best_text)):
                    best_text = cell_text
                    cell_idx = i + 1
            
            if best_text:
                # Try to split team name from affiliation
                # Common patterns: "Team Name University Name" or "Team NameUniversity"
                import re
                
                # Look for patterns that might separate team from university
                patterns = [
                    r'^(.*?)(National University|University|Institute|College|Tech)',
                    r'^(.*?[A-Z][a-z]+)([A-Z][a-z].*)',  # CamelCase split
                    r'^(.*?)([A-Z]{2,}.*)',  # Split at consecutive capitals
                ]
                
                team_name = best_text
                for pattern in patterns:
                    match = re.match(pattern, best_text)
                    if match:
                        team_name = match.group(1).strip()
                        team_affiliation = match.group(2).strip()
                        break
        
        team_data['team_name'] = team_name if team_name else 'Unknown Team'
        team_data['team_affiliation'] = team_affiliation
        print(f"  Team: '{team_data['team_name']}' Affiliation: '{team_affiliation}'")
        
        # Find where problems start by looking for cells with score classes
        problem_start_idx = cell_idx
        for i in range(cell_idx, len(cells)):
            cell_classes = cells[i].get('class', [])
            if any('score_' in cls for cls in cell_classes):
                problem_start_idx = i
                break
        
        print(f"  Problems start at cell index: {problem_start_idx}")
        
        # Parse problems
        team_data['problems'] = {}
        cell_idx = problem_start_idx
        
        for prob_idx, prob_name in enumerate(problems):
            if cell_idx < len(cells):
                prob_cell = cells[cell_idx]
                cell_text = prob_cell.get_text(strip=True)
                cell_classes = prob_cell.get('class', [])
                
                problem_info = {
                    'text': cell_text,
                    'status': 'unattempted',
                    'attempts': 0,
                    'time': None,
                    'classes': cell_classes
                }
                
                # Determine status based on classes
                class_str = ' '.join(cell_classes)
                if 'score_correct' in class_str:
                    problem_info['status'] = 'accepted'
                    # Parse attempts/time from text like "1/22"
                    if '/' in cell_text:
                        try:
                            parts = cell_text.split('/')
                            problem_info['attempts'] = int(parts[0])
                            problem_info['time'] = int(parts[1])
                        except (ValueError, IndexError):
                            pass
                    elif cell_text.isdigit():
                        problem_info['attempts'] = 1
                        problem_info['time'] = int(cell_text)
                elif 'score_incorrect' in class_str:
                    problem_info['status'] = 'wrong_answer'
                    # Extract attempts from text like "-2" or "2"
                    if cell_text.startswith('-'):
                        try:
                            problem_info['attempts'] = int(cell_text[1:])
                        except ValueError:
                            pass
                    elif cell_text.isdigit():
                        problem_info['attempts'] = int(cell_text)
                elif 'score_neutral' in class_str or not cell_text:
                    problem_info['status'] = 'unattempted'
                
                team_data['problems'][prob_name] = problem_info
                print(f"    {prob_name}: {cell_text} -> {problem_info['status']}")
                cell_idx += 1
        
        # Find score columns - look for cells that contain numeric scores
        # Work backwards from the end to find score-related cells
        remaining_cells = cells[cell_idx:]
        
        # The last few cells should contain score information
        score_found = False
        penalty_found = False
        
        # Look at last few cells for score patterns
        for i in range(len(remaining_cells) - 1, -1, -1):
            cell_text = remaining_cells[i].get_text(strip=True)
            cell_classes = remaining_cells[i].get('class', [])
            
            # Look for total score (usually a number)
            if not score_found and (cell_text.isdigit() or 
                                   any('total' in cls.lower() for cls in cell_classes) or
                                   any('score' in cls.lower() for cls in cell_classes)):
                team_data['total_score'] = cell_text
                score_found = True
            
            # Look for penalty/solved info (usually contains / or solved count)
            elif not penalty_found and ('/' in cell_text or 
                                       any('penalty' in cls.lower() for cls in cell_classes) or
                                       any('solved' in cls.lower() for cls in cell_classes)):
                team_data['solved_penalty'] = cell_text
                penalty_found = True
            
            if score_found and penalty_found:
                break
        
        # Fallback: use last two cells if we didn't find specific patterns
        if not score_found and not penalty_found and len(remaining_cells) >= 2:
            team_data['solved_penalty'] = remaining_cells[-2].get_text(strip=True)
            team_data['total_score'] = remaining_cells[-1].get_text(strip=True)
        elif not score_found and len(remaining_cells) >= 1:
            team_data['total_score'] = remaining_cells[-1].get_text(strip=True)
            team_data['solved_penalty'] = ''
        else:
            if not score_found:
                team_data['total_score'] = ''
            if not penalty_found:
                team_data['solved_penalty'] = ''
        
        print(f"  Score: '{team_data.get('total_score', 'N/A')}', Penalty: '{team_data.get('solved_penalty', 'N/A')}'")
        
        teams_data.append(team_data)
    
    return {
        'problems': problems,
        'teams': teams_data,
        'total_teams': len(teams_data),
        'column_structure': {
            'all_columns': all_column_ids,
            'problem_columns': problem_columns
        }
    }

def calculate_problem_statistics(data):
    """
    Calculate detailed statistics for each problem
    
    Args:
        data (dict): Parsed scoreboard data
    
    Returns:
        pd.DataFrame: Problem statistics
    """
    
    problem_stats = []
    total_teams = len(data['teams'])
    
    # Use all problems - don't exclude the last one if it's a valid problem
    problems_to_analyze = data['problems']
    
    for prob_name in problems_to_analyze:
        # Skip non-problem columns (but keep valid problem letters like L)
        if prob_name.lower() in ['team', 'rank', 'score', 'total', 'penalty', 'solved']:
            continue
            
        # Collect all attempts for this problem
        all_attempts = []
        solve_times = []
        total_attempts = 0
        teams_that_tried = 0
        teams_that_solved = 0
        
        for team in data['teams']:
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                if prob_data['status'] in ['accepted', 'wrong_answer']:
                    teams_that_tried += 1
                    attempts = prob_data.get('attempts', 0)
                    if attempts > 0:
                        total_attempts += attempts
                        all_attempts.append(attempts)
                
                if prob_data['status'] == 'accepted':
                    teams_that_solved += 1
                    solve_time = prob_data.get('time', 0)
                    if solve_time and solve_time > 0:
                        solve_times.append(solve_time)
        
        # Calculate statistics
        first_solve_time = min(solve_times) if solve_times else 0
        average_tries = sum(all_attempts) / len(all_attempts) if all_attempts else 0
        solve_percentage = (teams_that_solved / total_teams * 100) if total_teams > 0 else 0
        
        stats = {
            'Problem': prob_name,
            'Number_of_Solves': teams_that_solved,
            'First_Solve_Time': first_solve_time,
            'Solved_Tries_Ratio': f"{teams_that_solved}/{total_attempts}" if total_attempts > 0 else f"{teams_that_solved}/0",
            'Teams_That_Tried': teams_that_tried,
            'Total_Attempts': total_attempts,
            'Average_Tries': round(average_tries, 2),
            'Solve_Percentage': round(solve_percentage, 1),
            'Teams_That_Solved': teams_that_solved,
            'Teams_Total': total_teams
        }
        
        problem_stats.append(stats)
    
    return pd.DataFrame(problem_stats)

def create_detailed_problem_analysis(data):
    """
    Create a comprehensive problem analysis with team-by-team breakdown
    
    Args:
        data (dict): Parsed scoreboard data
    
    Returns:
        dict: Detailed analysis including statistics and team breakdowns
    """
    
    # Use all problems - filter out non-problem columns instead of excluding last column
    problems_to_analyze = [p for p in data['problems'] 
                          if p.lower() not in ['team', 'rank', 'score', 'total', 'penalty', 'solved']]
    
    analysis = {
        'problem_stats': calculate_problem_statistics(data),
        'problem_details': {}
    }
    
    for prob_name in problems_to_analyze:
        prob_details = {
            'accepted_teams': [],
            'wrong_teams': [],
            'unattempted_teams': [],
            'solve_times_distribution': [],
            'attempts_distribution': []
        }
        
        for team in data['teams']:
            team_name = team.get('team_name', 'Unknown')
            team_rank = team.get('rank', 'N/A')
            
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                if prob_data['status'] == 'accepted':
                    prob_details['accepted_teams'].append({
                        'team': team_name,
                        'rank': team_rank,
                        'attempts': prob_data.get('attempts', 1),
                        'time': prob_data.get('time', 0),
                        'text': prob_data.get('text', '')
                    })
                    
                    if prob_data.get('time', 0) > 0:
                        prob_details['solve_times_distribution'].append(prob_data['time'])
                    
                    if prob_data.get('attempts', 0) > 0:
                        prob_details['attempts_distribution'].append(prob_data['attempts'])
                
                elif prob_data['status'] == 'wrong_answer':
                    prob_details['wrong_teams'].append({
                        'team': team_name,
                        'rank': team_rank,
                        'attempts': prob_data.get('attempts', 1),
                        'text': prob_data.get('text', '')
                    })
                    
                    if prob_data.get('attempts', 0) > 0:
                        prob_details['attempts_distribution'].append(prob_data['attempts'])
                
                else:
                    prob_details['unattempted_teams'].append({
                        'team': team_name,
                        'rank': team_rank
                    })
            else:
                prob_details['unattempted_teams'].append({
                    'team': team_name,
                    'rank': team_rank
                })
        
        # Sort by solve time for accepted teams
        prob_details['accepted_teams'].sort(key=lambda x: x['time'])
        
        analysis['problem_details'][prob_name] = prob_details
    
    return analysis
    """
    Create a comprehensive DataFrame with all scoreboard data
    
    Args:
        data (dict): Parsed scoreboard data
    
    Returns:
        pd.DataFrame: Complete scoreboard data
    """
    
    rows = []
    
    for team in data['teams']:
        row = {
            'Rank': team.get('rank', ''),
            'Team_Name': team.get('team_name', ''),
            'Total_Score': team.get('total_score', ''),
            'Solved_Penalty': team.get('solved_penalty', ''),
            'Problems_Solved': sum(1 for p in team.get('problems', {}).values() if p['status'] == 'accepted'),
            'Problems_Attempted': sum(1 for p in team.get('problems', {}).values() if p['status'] in ['accepted', 'wrong_answer'])
        }
        
        # Add individual problem data
        for prob_name in data['problems']:
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                # Problem status (Accepted/Wrong/Unattempted)
                row[f'{prob_name}_Status'] = prob_data['status']
                
                # Attempts count
                row[f'{prob_name}_Attempts'] = prob_data.get('attempts', 0)
                
                # Solve time (if accepted)
                row[f'{prob_name}_Time'] = prob_data.get('time', '') if prob_data['status'] == 'accepted' else ''
                
                # Original cell text
                row[f'{prob_name}_Text'] = prob_data.get('text', '')
                
                # CSS classes (for debugging)
                row[f'{prob_name}_Classes'] = ','.join(prob_data.get('classes', []))
            else:
                # Problem not found for this team
                row[f'{prob_name}_Status'] = 'unattempted'
                row[f'{prob_name}_Attempts'] = 0
                row[f'{prob_name}_Time'] = ''
                row[f'{prob_name}_Text'] = ''
                row[f'{prob_name}_Classes'] = ''
        
        rows.append(row)
    
    return pd.DataFrame(rows)

def create_summary_dataframe(data):
    """
    Create a clean summary DataFrame for basic analysis
    
    Args:
        data (dict): Parsed scoreboard data
    
    Returns:
        pd.DataFrame: Summary scoreboard data
    """
    
    rows = []
    
    for team in data['teams']:
        row = {
            'Rank': team.get('rank', ''),
            'Team_Name': team.get('team_name', ''),
            'Team_Affiliation': team.get('team_affiliation', ''),
            'Total_Score': team.get('total_score', ''),
            'Solved_Penalty': team.get('solved_penalty', ''),
            'Problems_Solved': sum(1 for p in team.get('problems', {}).values() if p['status'] == 'accepted'),
            'Problems_Attempted': sum(1 for p in team.get('problems', {}).values() if p['status'] in ['accepted', 'wrong_answer'])
        }
        
        # Use all problems - filter out non-problem columns instead
        problems_to_use = [p for p in data['problems'] 
                          if p.lower() not in ['team', 'rank', 'score', 'total', 'penalty', 'solved']]
        
        # Add problem results in contest format (attempts/time or -attempts)
        for prob_name in problems_to_use:
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                if prob_data['status'] == 'accepted':
                    attempts = prob_data.get('attempts', 1)
                    time = prob_data.get('time', '')
                    row[f'Problem_{prob_name}'] = f"{attempts}/{time}" if time else prob_data.get('text', '+')
                elif prob_data['status'] == 'wrong_answer':
                    attempts = prob_data.get('attempts', 1)
                    row[f'Problem_{prob_name}'] = f"-{attempts}" if attempts > 0 else prob_data.get('text', '-')
                else:
                    row[f'Problem_{prob_name}'] = ''
            else:
                row[f'Problem_{prob_name}'] = ''
        
        rows.append(row)
    
    return pd.DataFrame(rows)

def create_comprehensive_dataframe(data):
    """
    Create a comprehensive DataFrame with all scoreboard data
    
    Args:
        data (dict): Parsed scoreboard data
    
    Returns:
        pd.DataFrame: Complete scoreboard data
    """
    
    rows = []
    
    for team in data['teams']:
        row = {
            'Rank': team.get('rank', ''),
            'Team_Name': team.get('team_name', ''),
            'Team_Affiliation': team.get('team_affiliation', ''),
            'Total_Score': team.get('total_score', ''),
            'Solved_Penalty': team.get('solved_penalty', ''),
            'Problems_Solved': sum(1 for p in team.get('problems', {}).values() if p['status'] == 'accepted'),
            'Problems_Attempted': sum(1 for p in team.get('problems', {}).values() if p['status'] in ['accepted', 'wrong_answer'])
        }
        
        # Use all problems - filter out non-problem columns instead
        problems_to_use = [p for p in data['problems'] 
                          if p.lower() not in ['team', 'rank', 'score', 'total', 'penalty', 'solved']]
        
        # Add individual problem data
        for prob_name in problems_to_use:
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                # Problem status (Accepted/Wrong/Unattempted)
                row[f'{prob_name}_Status'] = prob_data['status']
                
                # Attempts count
                row[f'{prob_name}_Attempts'] = prob_data.get('attempts', 0)
                
                # Solve time (if accepted)
                row[f'{prob_name}_Time'] = prob_data.get('time', '') if prob_data['status'] == 'accepted' else ''
                
                # Original cell text
                row[f'{prob_name}_Text'] = prob_data.get('text', '')
                
                # CSS classes (for debugging)
                row[f'{prob_name}_Classes'] = ','.join(prob_data.get('classes', []))
            else:
                # Problem not found for this team
                row[f'{prob_name}_Status'] = 'unattempted'
                row[f'{prob_name}_Attempts'] = 0
                row[f'{prob_name}_Time'] = ''
                row[f'{prob_name}_Text'] = ''
                row[f'{prob_name}_Classes'] = ''
        
        rows.append(row)
    
    return pd.DataFrame(rows)

def save_to_csv(data, filename_prefix="icpc_scoreboard"):
    """
    Save scoreboard data to CSV files including problem statistics
    
    Args:
        data (dict): Parsed scoreboard data
        filename_prefix (str): Prefix for output files
    
    Returns:
        dict: Dictionary with filenames of created files
    """
    
    files_created = {}
    
    # 1. Comprehensive data with all details
    comprehensive_df = create_comprehensive_dataframe(data)
    comprehensive_filename = f"{filename_prefix}_comprehensive.csv"
    comprehensive_df.to_csv(comprehensive_filename, index=False)
    files_created['comprehensive'] = comprehensive_filename
    print(f"✓ Saved comprehensive data to: {comprehensive_filename}")
    
    # 2. Summary data (contest format)
    summary_df = create_summary_dataframe(data)
    summary_filename = f"{filename_prefix}_summary.csv"
    summary_df.to_csv(summary_filename, index=False)
    files_created['summary'] = summary_filename
    print(f"✓ Saved summary data to: {summary_filename}")
    
    # 3. Problem statistics
    problem_stats_df = calculate_problem_statistics(data)
    problem_stats_filename = f"{filename_prefix}_problem_statistics.csv"
    problem_stats_df.to_csv(problem_stats_filename, index=False)
    files_created['problem_statistics'] = problem_stats_filename
    print(f"✓ Saved problem statistics to: {problem_stats_filename}")
    
    # 4. Team statistics
    stats_data = []
    for team in data['teams']:
        problems = team.get('problems', {})
        solved = sum(1 for p in problems.values() if p['status'] == 'accepted')
        attempted = sum(1 for p in problems.values() if p['status'] in ['accepted', 'wrong_answer'])
        wrong = sum(1 for p in problems.values() if p['status'] == 'wrong_answer')
        total_attempts = sum(p.get('attempts', 0) for p in problems.values())
        
        stats_data.append({
            'Rank': team.get('rank', ''),
            'Team_Name': team.get('team_name', ''),
            'Team_Affiliation': team.get('team_affiliation', ''),
            'Total_Score': team.get('total_score', ''),
            'Problems_Solved': solved,
            'Problems_Wrong': wrong,
            'Problems_Attempted': attempted,
            'Total_Attempts': total_attempts,
            'Success_Rate': f"{(solved/attempted*100):.1f}%" if attempted > 0 else "0%"
        })
    
    stats_df = pd.DataFrame(stats_data)
    stats_filename = f"{filename_prefix}_team_statistics.csv"
    stats_df.to_csv(stats_filename, index=False)
    files_created['team_statistics'] = stats_filename
    print(f"✓ Saved team statistics to: {stats_filename}")
    
    # 5. Problem summary across all teams
    problem_summary_data = []
    
    # Use all problems - filter out non-problem columns instead of excluding last column
    problems_to_analyze = [p for p in data['problems'] 
                          if p.lower() not in ['team', 'rank', 'score', 'total', 'penalty', 'solved']]
    
    for prob_name in problems_to_analyze:
        # Collect statistics for this problem
        accepted_teams = []
        wrong_teams = []
        unattempted_teams = []
        solve_times = []
        attempts_list = []
        total_attempts = 0
        
        for team in data['teams']:
            team_name = team.get('team_name', 'Unknown')
            team_rank = team.get('rank', 'N/A')
            
            if prob_name in team.get('problems', {}):
                prob_data = team['problems'][prob_name]
                
                if prob_data['status'] == 'accepted':
                    accepted_teams.append(team_name)
                    if prob_data.get('time', 0) > 0:
                        solve_times.append(prob_data['time'])
                    if prob_data.get('attempts', 0) > 0:
                        attempts_list.append(prob_data['attempts'])
                        total_attempts += prob_data['attempts']
                
                elif prob_data['status'] == 'wrong_answer':
                    wrong_teams.append(team_name)
                    if prob_data.get('attempts', 0) > 0:
                        attempts_list.append(prob_data['attempts'])
                        total_attempts += prob_data['attempts']
                
                else:
                    unattempted_teams.append(team_name)
            else:
                unattempted_teams.append(team_name)
        
        # Calculate summary statistics
        num_teams = len(data['teams'])
        num_solved = len(accepted_teams)
        num_wrong = len(wrong_teams)
        num_unattempted = len(unattempted_teams)
        num_tried = num_solved + num_wrong
        
        first_solve_time = min(solve_times) if solve_times else 0
        avg_solve_time = sum(solve_times) / len(solve_times) if solve_times else 0
        avg_attempts = sum(attempts_list) / len(attempts_list) if attempts_list else 0
        
        solve_percentage = (num_solved / num_teams * 100) if num_teams > 0 else 0
        try_percentage = (num_tried / num_teams * 100) if num_teams > 0 else 0
        
        problem_summary_data.append({
            'Problem': prob_name,
            'Total_Teams': num_teams,
            'Teams_Solved': num_solved,
            'Teams_Wrong_Answer': num_wrong,
            'Teams_Unattempted': num_unattempted,
            'Teams_That_Tried': num_tried,
            'Solve_Percentage': round(solve_percentage, 1),
            'Try_Percentage': round(try_percentage, 1),
            'First_Solve_Time': first_solve_time,
            'Average_Solve_Time': round(avg_solve_time, 1),
            'Total_Attempts': total_attempts,
            'Average_Attempts': round(avg_attempts, 1),
            'Solved_Tries_Ratio': f"{num_solved}/{total_attempts}",
            'Success_Rate': round((num_solved / num_tried * 100), 1) if num_tried > 0 else 0.0,
            'Difficulty_Ranking': 0  # Will be filled after sorting
        })
    
    # Sort by solve percentage to determine difficulty ranking
    problem_summary_data.sort(key=lambda x: x['Solve_Percentage'], reverse=True)
    for i, prob in enumerate(problem_summary_data):
        prob['Difficulty_Ranking'] = i + 1  # 1 = easiest, higher = harder
    
    # Sort back by problem name
    problem_summary_data.sort(key=lambda x: x['Problem'])
    
    problem_summary_df = pd.DataFrame(problem_summary_data)
    problem_summary_filename = f"{filename_prefix}_problem_summary.csv"
    problem_summary_df.to_csv(problem_summary_filename, index=False)
    files_created['problem_summary'] = problem_summary_filename
    print(f"✓ Saved problem summary across all teams to: {problem_summary_filename}")
    
    return files_created

def display_scoreboard_summary(data):
    """Display a summary of the parsed scoreboard"""
    print("\n" + "="*60)
    print("SCOREBOARD SUMMARY")
    print("="*60)
    print(f"Total teams: {data['total_teams']}")
    print(f"Total problems: {len(data['problems'])}")
    print(f"Problems: {data['problems']}")
    
    print(f"\nTop 10 teams:")
    for i, team in enumerate(data['teams'][:10]):
        solved_count = sum(1 for p in team.get('problems', {}).values() if p['status'] == 'accepted')
        print(f"{team.get('rank', '?'):>3}. {team.get('team_name', 'Unknown'):<30} | Solved: {solved_count:>2} | Score: {team.get('total_score', 'N/A')}")

# Example usage
if __name__ == "__main__":
    url = "https://competition.binus.ac.id/icpc2017/final.html"
    
    # First debug the structure
    print("=== DEBUGGING TABLE STRUCTURE ===")
    debug_table_structure(url)
    print("\n" + "="*50 + "\n")
    
    try:
        print("Parsing scoreboard...")
        scoreboard_data = parse_icpc_scoreboard(url)
        
        print("\nDisplaying summary...")
        display_scoreboard_summary(scoreboard_data)
        
        # Show problem statistics
        print("\n" + "="*60)
        print("PROBLEM STATISTICS SUMMARY")
        print("="*60)
        problem_stats = calculate_problem_statistics(scoreboard_data)
        print(problem_stats.to_string(index=False))
        
        print(f"\nSaving data to CSV files...")
        files_created = save_to_csv(scoreboard_data, "icpc_jakarta_2017")
        
        print(f"\n📁 Files created:")
        for file_type, filename in files_created.items():
            print(f"  {file_type.replace('_', ' ').title()}: {filename}")
        
        # Preview the problem summary
        if 'problem_summary' in files_created:
            print(f"\n📊 Problem Summary Preview:")
            summary_df = pd.read_csv(files_created['problem_summary'])
            # Show key columns for preview
            preview_cols = ['Problem', 'Teams_Solved', 'Solve_Percentage', 'Teams_That_Tried', 'Average_Attempts', 'Difficulty_Ranking']
            available_cols = [col for col in preview_cols if col in summary_df.columns]
            print(summary_df[available_cols].head(10).to_string(index=False))
        
        print(f"\n🎉 Successfully parsed {scoreboard_data['total_teams']} teams and {len(scoreboard_data['problems'])} problems!")
        print(f"📊 Generated comprehensive problem summary including problem L")
        
    except Exception as e:
        print(f"❌ Error: {e}")
        import traceback
        traceback.print_exc()