def create_sample(file):
    with open(file) as fs:
        s1 = eval(fs.readline().strip())
        s2 = eval(fs.readline().strip())
        return list(zip(s1, s2))

tuples = create_sample("./Tests/test4.td")

with open('./Tests/test4.td', 'w') as f:
    for tup in tuples:
        print(f"{tup[0]} {tup[1]}", file=f)
