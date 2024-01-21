import cs50

# TODO
count = 0
while True:
    height = cs50.get_int("Height: ")
    if height >= 1 and height <= 8:
        break
while count != height:
    count += 1
    print(" " * (height - count), end="")
    print("#" * count)
