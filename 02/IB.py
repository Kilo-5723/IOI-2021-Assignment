def check(n, m):
    if n == 0:
        return True
    if n % 10 != m % 2:
        return False
    return check(n//10, m//2)


input = open("binary.in", "r")
output = open("binary.out", "w")

que = [0]
ava = [0]
step = 1
cnt = int(input.read())
las = 0
while len(que) <= cnt:
    tmp = []
    for a in ava:
        if (check(a+step, a+step)):
            tmp.append(a)
    ava = tmp
    tmp = []
    for a in ava:
        if check(a+step, a+step):
            tmp.append(a+step)
    que += tmp
    ava += tmp
    step *= 10
output.write(str(que[cnt]))
