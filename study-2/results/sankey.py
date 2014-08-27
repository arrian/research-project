# Generates the JSON for a sankey graph.

def generateJson(title, data):

	nodes = []
	nodes.append(["high", "medium", "low"])
	nodes.append(["high", "medium", "low"])
	nodes.append(["high", "medium", "low"])

	nodeStrings = []
	for node in nodes:
		for item in node:
			nodeStrings.append('  {"name":"' + item + '"}')

	linkStrings = []
	count = 0
	for k,v in data.items():
		split = k.split('-')

		prev = -1
		init = -1

		for i in range(0,3):
			current = nodes[i].index(split[i]) + i * len(nodes[i])

			if init is -1:
				init = current

			if prev >= 0:
				linkStrings.append('  {"source":' + str(prev) + ',"target":' + str(current) + ',"value":' + str(v) + ',"init":' + str(init) + '}')
			prev = current
		count += 1

	print('{"title" : ' + title + ',\n "nodes":[')
	print(",\n".join(nodeStrings))
	print('],')
	print('"links":[')
	print(",\n".join(linkStrings))
	print(']}')


data = {}
data["high-high-high"] = 9
data["high-high-medium"] = 2
data["high-medium-medium"] = 1
data["low-high-high"] = 1
data["low-high-medium"] = 1
data["low-low-medium"] = 1
data["low-medium-high"] = 1
data["low-medium-low"] = 1
data["low-medium-medium"] = 4
data["medium-high-high"] = 5
data["medium-high-low"] = 1
data["medium-high-medium"] = 6
data["medium-medium-high"] = 3
data["medium-medium-medium"] = 4

generateJson("Aesthetic Enjoyment", data)

print('-------------------')

data = {}
data["high-high-high"] = 6
data["high-low-medium"] = 1
data["high-medium-high"] = 2
data["high-medium-medium"] = 2
data["low-high-high"] = 1
data["low-high-medium"] = 1
data["low-low-low"] = 2
data["low-low-medium"] = 2
data["low-medium-high"] = 1
data["low-medium-medium"] = 3
data["medium-high-high"] = 2
data["medium-high-medium"] = 7
data["medium-low-low"] = 2
data["medium-medium-high"] = 1
data["medium-medium-medium"] = 7

generateJson("Didactic Enjoyment", data)

print('-------------------')

data = {}
data["high-high-high"] = 1
data["high-low-low"] = 1
data["high-medium-high"] = 1
data["high-medium-low"] = 1
data["high-medium-medium"] = 2
data["low-low-low"] = 6
data["low-low-medium"] = 1
data["low-medium-high"] = 1
data["low-medium-medium"] = 4
data["medium-high-high"] = 4
data["medium-high-medium"] = 3
data["medium-low-low"] = 1
data["medium-medium-high"] = 1
data["medium-medium-medium"] = 11
# data["medium-unspecified-unspecified"] = 1
# data["unspecified-unspecified-unspecified"] = 1

generateJson("Aesthetic Understanding", data)

print('-------------------')

data = {}
data["high-low-high"] = 1
data["high-medium-high"] = 1
data["high-medium-low"] = 1
data["high-medium-medium"] = 1
data["low-low-low"] = 6
data["low-low-medium"] = 2
data["low-medium-high"] = 2
data["low-medium-low"] = 1
data["low-medium-medium"] = 7
data["medium-high-high"] = 2
data["medium-low-low"] = 1
data["medium-medium-medium"] = 13
# data["unspecified-unspecified-unspecified"] = 2

generateJson("Didactic Understanding", data)

print('-------------------')
