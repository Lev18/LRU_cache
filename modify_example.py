def create_list(file):
    with open(file,"r+") as f:
        s = f.readline()
        lst = s.split(",")
        print(*lst, file=f, sep = '\n')

create_list("./Outputs/test4.ex")
