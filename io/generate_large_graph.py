from random import randint
from functools import reduce

N_CLASSES = 200
N_STUDENTS = 1000

def choose_classes_vary(classes, lower, upper):
    num_classes = randint(lower, upper)
    inds = [randint(0,N_CLASSES-1) for i in range(num_classes)]
    return [classes[ind] for ind in inds]

def choose_classes(classes, num_classes):
    inds = [randint(0, N_CLASSES-1) for i in range(num_classes)]
    return [classes[ind] for ind in inds]

def classes_str(classes):
    return reduce(lambda acc, cls_str : acc + cls_str,
            map(lambda cls: "C, " + cls + "; ", classes))

def generate_graph_file(fname, density):
    with open(fname, 'w') as wfile:
        map(lambda cls : wfile.write("C, " + cls + "; "), classes)
        map(lambda std : wfile.write("S, " + std + "; "), students)
        wfile.write("\n")
        map(lambda std : wfile.write("S, " + std + "; " +
              classes_str(choose_classes(classes, density)) + "\n"),
            students)

# create a graph-file consisting of 200 courses and 1000 students
# edges are randomly generated between students and classes
classes = map(lambda ind : "class " + str(ind), range(N_CLASSES))
students = map(lambda ind : "student " + str(ind), range(N_STUDENTS))

sparse_fname = "large_graph_sparse.txt"
generate_graph_file(sparse_fname, 4)

mid_fname = "large_graph_mid.txt"
generate_graph_file(mid_fname, 8)

dense_fname = "large_graph_dense.txt"
generate_graph_file(dense_fname, 16)
