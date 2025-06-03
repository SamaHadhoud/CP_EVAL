s = input().strip()
t = ''.join('1' if c == '0' else '0' for c in s)
print(t)