def main():
    import sys
    data = sys.stdin.read().splitlines()
    n = int(data[0])
    s_list = []
    for i in range(1, n + 1):
        s_list.append(data[i].strip())
    
    current = True
    for i in range(n - 2, -1, -1):
        report = s_list[i + 1]
        R = (report == "TRUTH")
        current = (current == R)
    
    R0 = (s_list[0] == "TRUTH")
    result = (current == R0)
    print("TRUTH" if result else "LIE")

if __name__ == "__main__":
    main()