# Generates the JSON for a sankey graph.

levels = ["high", "medium", "low"]


import itertools
preferences = itertools.product(levels, repeat = 3)

def generateJson(title, data):

	nodes = []
	nodes.append(levels)# Beginning
	nodes.append(levels)# Middle
	nodes.append(levels)# End

	nodeStrings = []
	for node in nodes:
		for item in node:
			nodeStrings.append('  {"name":"' + item + '"}')

	linkStrings = []
	count = 0
	for k,v in data.items():
		split = k.split('-')
		print(split,v)

		prev = -1
		init = nodes[0].index(split[0])

		for i in range(0,3):
			current = nodes[i].index(split[i]) + i * len(nodes[i])

			if prev >= 0 and v > 0:
				linkStrings.append('  {"source":' + str(prev) + ',"target":' + str(current) + ',"value":' + str(v) + ',"init":' + str(init) + '}')
			prev = current
		count += 1

	print('{"title" : "' + title + '",\n "nodes":[')
	print(",\n".join(nodeStrings))
	print('],')
	print('"links":[')
	print(",\n".join(linkStrings))
	print(']}')

content = []

with open("results.txt") as f:
    content = f.readlines()

counter = 0

results = []

tags = []
for pref in preferences:
	tags.append('-'.join(pref))

# print(tags)

for i in range(0,4):
	results.append({})
	for t in tags:
		results[i][t] = 0

for line in content:
	tokens = line.replace('\n','').split(',')

	entry = ''
	tcounter = 0
	gcounter = 0
	for token in tokens:
		if tcounter is not 0:
			entry = entry + '-'
		
		entry = entry + token.lower()
		if tcounter == 2:
			results[gcounter][entry] = results[gcounter][entry] + 1
			tcounter = 0
			gcounter = gcounter + 1
			entry = ''
		else:
			tcounter += 1


	counter = 0

# print(results[0])
generateJson("Enjoyment with No Visualisation", results[0])
print('-------------------')
generateJson("Understanding with No Visualisation", results[1])
print('-------------------')
generateJson("Enjoyment with Visualisation", results[2])
print('-------------------')
generateJson("Understanding with Visualisation", results[3])


# data = {}
# data["high-high-high"] = 9
# data["high-high-medium"] = 2
# data["high-medium-medium"] = 1
# data["low-high-high"] = 1
# data["low-high-medium"] = 1
# data["low-low-medium"] = 1
# data["low-medium-high"] = 1
# data["low-medium-low"] = 1
# data["low-medium-medium"] = 4
# data["medium-high-high"] = 5
# data["medium-high-low"] = 1
# data["medium-high-medium"] = 6
# data["medium-medium-high"] = 3
# data["medium-medium-medium"] = 4

# generateJson("Aesthetic Enjoyment", data)

# print('-------------------')


# data = {}
# data["high-high-high"] = 9
# data["high-high-medium"] = 2
# data["high-medium-medium"] = 1
# data["low-high-high"] = 1
# data["low-high-medium"] = 1
# data["low-low-medium"] = 1
# data["low-medium-high"] = 1
# data["low-medium-low"] = 1
# data["low-medium-medium"] = 4
# data["medium-high-high"] = 5
# data["medium-high-low"] = 1
# data["medium-high-medium"] = 6
# data["medium-medium-high"] = 3
# data["medium-medium-medium"] = 4

# generateJson("Aesthetic Enjoyment", data)

# print('-------------------')

# data = {}
# data["high-high-high"] = 6
# data["high-low-medium"] = 1
# data["high-medium-high"] = 2
# data["high-medium-medium"] = 2
# data["low-high-high"] = 1
# data["low-high-medium"] = 1
# data["low-low-low"] = 2
# data["low-low-medium"] = 2
# data["low-medium-high"] = 1
# data["low-medium-medium"] = 3
# data["medium-high-high"] = 2
# data["medium-high-medium"] = 7
# data["medium-low-low"] = 2
# data["medium-medium-high"] = 1
# data["medium-medium-medium"] = 7

# generateJson("Didactic Enjoyment", data)

# print('-------------------')

# data = {}
# data["high-high-high"] = 1
# data["high-low-low"] = 1
# data["high-medium-high"] = 1
# data["high-medium-low"] = 1
# data["high-medium-medium"] = 2
# data["low-low-low"] = 6
# data["low-low-medium"] = 1
# data["low-medium-high"] = 1
# data["low-medium-medium"] = 4
# data["medium-high-high"] = 4
# data["medium-high-medium"] = 3
# data["medium-low-low"] = 1
# data["medium-medium-high"] = 1
# data["medium-medium-medium"] = 11
# # data["medium-unspecified-unspecified"] = 1
# # data["unspecified-unspecified-unspecified"] = 1

# generateJson("Aesthetic Understanding", data)

# print('-------------------')

# data = {}
# data["high-low-high"] = 1
# data["high-medium-high"] = 1
# data["high-medium-low"] = 1
# data["high-medium-medium"] = 1
# data["low-low-low"] = 6
# data["low-low-medium"] = 2
# data["low-medium-high"] = 2
# data["low-medium-low"] = 1
# data["low-medium-medium"] = 7
# data["medium-high-high"] = 2
# data["medium-low-low"] = 1
# data["medium-medium-medium"] = 13
# # data["unspecified-unspecified-unspecified"] = 2

# generateJson("Didactic Understanding", data)

# print('-------------------')
