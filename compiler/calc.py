import os

path = ""
count = 0
files = os.listdir(".")
print("分析的文件:")
for file in files:
    if file[-1] in ['h', 'p']:
        length = len(open(file, 'r', encoding='UTF-8').readlines())
        print("{}: {}".format(file, length))
        count += length

print("总行数为:{}".format(count))