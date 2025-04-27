with open("words.txt", "r") as f:
    words = []
    for i in range(8):
        words.append([])
        for y in range(8):
            line = f.readline()
            words[i].append( line.split())


for index, section in enumerate(words):
    print("section: " + str(index))
    for word in section:
        for character in word:
            print(chr(int(character, 0)), end='')
        print()
