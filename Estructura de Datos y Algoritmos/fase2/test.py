elems1 = [5, 12, 2, 1, 3, 9, 7]
elems2 = [9, 3, 21, 4, 7, 23]


for elem in elems2:
    if elem not in elems1:
        elems1.append(elem)

print(sorted(elems1))