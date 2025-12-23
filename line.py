import glob

files = glob.glob("include/**/*.hpp", recursive=True)
files += glob.glob("src/**/*.cpp", recursive=True)

count = 0

for f in files:
  with open(f) as fs:
    count += len(fs.readlines())

print(count)