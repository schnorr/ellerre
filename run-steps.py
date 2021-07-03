#!/usr/bin/python3

"""

To launch the Python script, you can input the step .dot file or load
it using the GUI menu. You can also save the generated images step by
step using the menu -> File option.

#+begin_src shell :results output :exports both
python runanimation.py ./build/LR0-step.dot
#+end_src

The program renders the step files using the .dot type, with positions
set automatically. You can use fixed nodes position by passing the =fdp=
argument to the execution, as follows.

"""
import os,sys,re
from re import search

import tempfile
import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk

import pygraphviz as pgv

# Default graph reading type
type = "dot"
count = 0

def natural_sort(l): 
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [convert(c) for c in re.split('([0-9]+)', key)]
    return sorted(l, key=alphanum_key)

def get_nodes_position(file_name):
    positions = []
    G = pgv.AGraph(file_name)
    for state in G.iternodes():
        positions.append(state.attr['pos'])
        # G.graph_attr.update(size="2,2")

    return positions

# Read any file with dot (it shows all hidden attributes) and save as a dot file
def read_with_dot(tmp_dir):
    step = 0 
    for file in natural_sort(os.listdir(tmp_dir)):
        if file.startswith("LR-step"):
            run_line = "dot " + str(tmp_dir) + "/LR-step-" + str(step) + " > " + str(tmp_dir) + "/" + str(step) + ".dot"
            os.system(run_line)
            step = step + 1

    return step-1

# Partitionate the steps file for each step
def create_file_steps(file_name, tmp_dir):
    step=0
    f1 = open(str(tmp_dir) + "/LR-step-0", "w")
    with open(file_name) as f:
        for line in f:       
            if(search ("step=" + str(step+1), line.strip())):
                f1.close()
                step = step+1
                f1 = open(str(tmp_dir) + "/LR-step-" + str(step), "w")
            
            f1.write(line)       
    f1.close()
    

# Dot files processing to generate the images
# Rendering the graph as fdp - for undirected graphs
def create_fdp_images(tmp_dir):

    step = read_with_dot(tmp_dir)
    
    # Get positions from the last image
    last_pos = get_nodes_position(str(tmp_dir) + "/" + str(step) + ".dot")

    # Set nodes positions and generate png images
    step = 0
    for file in natural_sort(os.listdir(tmp_dir)):
        i = 0
        if file.endswith(".dot"):
            G = pgv.AGraph(tmp_dir + "/" + file)
            for state in G.iternodes():
                if(state == "nowhere"):
                    state.attr['pos'] = last_pos[-1] + "!"
                else:
                    state.attr['pos'] = last_pos[i] + "!"
                    i = i + 1
            # Create png temporary images
            G.draw(tmp_dir + "/" + str(step) + "-out.png", format="png", prog="fdp")
            step = step + 1

# Dot files processing to generate the images
# Rendering the graph as dot - for directed graphs
def create_dot_images(tmp_dir):

    step = read_with_dot(tmp_dir)
    
    step = 0
    for file in natural_sort(os.listdir(tmp_dir)):
        if file.endswith(".dot"):
            G = pgv.AGraph(tmp_dir + "/" + file)
            # Create png temporary images
            G.draw(tmp_dir + "/" + str(step) + "-out.png", format="png", prog="dot")
            step = step + 1

# Tkinter canva
class MainCanva(tk.Tk):
    
    def __init__(self, *args, **kwargs):

        tk.Tk.__init__(self, *args, **kwargs)
        self.title('EllErre - Step-by-step visualization for LR0 and LR1')
        self.menu = tk.Menu(self)
        self.config(menu=self.menu)

        # Create the menu
        fileMenu = tk.Menu(self.menu)
        fileMenu.add_command(label="Load .dot File",  command=self.load_dot)
        fileMenu.add_command(label="Save current image", command=self.save_image)
        fileMenu.add_command(label="Exit", command=self.close_app)
        self.menu.add_cascade(label="File", menu=fileMenu)
        self.menu.add_command(label="Previous", command=self.on_click_previous, state=tk.DISABLED)
        self.menu.add_command(label="Next", command=self.on_click_next, state=tk.DISABLED)
        self.menu.add_command(label="Reset", command=self.on_click_reset, state=tk.DISABLED)

        # Create canva
        self.canvas = tk.Canvas(self, bg='white')
        self.canvas.config(width=1150, height=600)
        self.canvas.pack(fill=tk.BOTH, expand=1)
        self.myimg = None

        # Create a temporary directory to store the step by step temporary images
        self.tmp_dir = tempfile.TemporaryDirectory(dir = ".")

        # Check if user input a file by command line
        if(len(sys.argv) > 1):

            file_name = sys.argv[1]

            global type
            type = "dot"
            if(len(sys.argv) == 3):
                type = sys.argv[2]
            
            # Create dot files and images
            create_file_steps(file_name, self.tmp_dir.name)
            # Create images as dot or fdp
            if(type == "fdp"):
                create_fdp_images(self.tmp_dir.name)
            else:
                create_dot_images(self.tmp_dir.name)
            
            # Activate buttons
            self.menu.entryconfig(3, state=tk.NORMAL)
            self.menu.entryconfig(4, state=tk.NORMAL)

            # Resize canva based on the input image    
            self.canvas.bind("<Configure>", self.resize)
            self.first_image = self.tmp_dir.name + "/0-out.png"
            self.img = ImageTk.PhotoImage(Image.open(self.first_image))

            # Load first image
            self.myimg = self.canvas.create_image((0, 0), image=self.img, anchor="nw")

        # Open the canva
        self.mainloop()
        self.tmp_dir.cleanup()

    def load_dot(self):
        global count, img, type
        file_name =  filedialog.askopenfilename(initialdir = ".",title = "Select file", filetypes=[("dot files","*.dot")])

        if(file_name):
            # Activate buttons and disable "previous" if set
            self.menu.entryconfig(2, state=tk.DISABLED)
            self.menu.entryconfig(3, state=tk.NORMAL)
            self.menu.entryconfig(4, state=tk.NORMAL)
            count = 0

            self.tmp_dir = tempfile.TemporaryDirectory(dir = ".")
            create_file_steps(file_name, self.tmp_dir.name)
            # Create images as dot or fdp
            print(type)
            if(type == "fdp"):
                create_fdp_images(self.tmp_dir.name)
            else:
                create_dot_images(self.tmp_dir.name)
            self.first_image = self.tmp_dir.name + "/0-out.png"

            img = ImageTk.PhotoImage(Image.open(self.first_image))
            self.myimg = self.canvas.create_image((0, 0), image=img, anchor="nw")
            self.canvas.itemconfig(self.myimg, image = img)

    def on_click_previous(self):
        global img, count
        count = count - 1
        filename = self.tmp_dir.name + '/' + str(count) + '-out.png'

        # Check if thereis a previous image, if its the first, disable the "previous" button
        if(count > 0):
            img = tk.PhotoImage(file=filename)
            self.menu.entryconfig(2, state=tk.NORMAL)
            self.menu.entryconfig(3, state=tk.NORMAL)
        else:
            img = tk.PhotoImage(file=filename)
            self.menu.entryconfig(2, state=tk.DISABLED)
        
        self.canvas.itemconfig(self.myimg, image = img)

    def on_click_next(self):
        global img, count
        count = count + 1
        filename = self.tmp_dir.name + '/' + str(count) + '-out.png'

        # Check if there is a next image, if not disable the "next" button
        if(os.path.exists(filename)):
            img = tk.PhotoImage(file=filename)
            self.menu.entryconfig(2, state=tk.NORMAL)
            self.canvas.itemconfig(self.myimg, image = img)
        else:
            self.menu.entryconfig(3, state=tk.DISABLED)

    def on_click_reset(self):
        global img, count
        count=0

        # Enable "next" and disable "previous", since it's the first image
        self.menu.entryconfig(3, state=tk.NORMAL)
        self.menu.entryconfig(2, state=tk.DISABLED)

        img = tk.PhotoImage(file=self.first_image)
        self.canvas.itemconfig(self.myimg, image = img)

    def resize(self, event):
        global count
        filename = self.tmp_dir.name + '/' + str(count) + '-out.png'

        img = Image.open(filename).resize((event.width, event.height), Image.ANTIALIAS)
        self.img = ImageTk.PhotoImage(img)
        self.canvas.itemconfig(self.myimg, image=self.img)

    def save_image(self):

        global count
        filename = self.tmp_dir.name + '/' + str(count) + '-out.png'
        abs_path = os.path.abspath(filename)
        
        user_filename = filedialog.asksaveasfile(mode = 'w', defaultextension=".png", initialfile = str(count) + '-out.png')
        
        image = Image.open(abs_path)
        image.save(user_filename.name)
        image.close()

    def close_app(self):
        self.destroy()


if __name__ == "__main__":
    main_canva = MainCanva()