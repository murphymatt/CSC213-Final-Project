from random import randint, sample
from functools import reduce

N_CLASSES = 200
N_STUDENTS = 1000

def choose_classes(classes):
    num_classes = randint(3,6)
    inds = sample(range(N_CLASSES), num_classes)
    return [classes[ind] for ind in inds]

def classes_str(classes):
    return reduce(lambda acc, cls_str : acc + cls_str,
            map(lambda cls: "C, " + cls + "; ", classes))

# create a graph-file consisting of 200 courses and 1000 students
# edges are randomly generated between students and classes
classes = map(lambda ind : "class " + str(ind), range(N_CLASSES))
students = map(lambda ind : "student " + str(ind), range(N_STUDENTS))

fname = "large_graph.txt"
with open(fname, 'w') as wfile:
    map(lambda cls : wfile.write("C, " + cls + "; "), classes)
    map(lambda std : wfile.write("S, " + std + "; "), students)
    wfile.write("\n")
    map(lambda std : wfile.write("S, " + std + "; " +
          classes_str(choose_classes(classes)) + "\n"),
        students)
