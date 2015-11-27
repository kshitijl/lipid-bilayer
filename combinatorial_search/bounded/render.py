instr = "144; 145; 146; 147; 148; 149; 150; 151; 152; 153; 154; 157; 158; 176; 177; 178; 179; 180; 181; 182; 183; 184; 185; 186; 189; 190;"

names = map(int, filter(lambda x: x, instr.split(";")))

a,b,c = 9,8,4

for name in names:
    k = name % c
    j = (name/c) % b
    i = name/(b*c)
    print "%d,%d,%d;" % (i,j,k),