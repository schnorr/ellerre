import os,sys
import tkinter as tk
from re import search
import tempfile
from PIL import Image, ImageTk
from tkinter import filedialog

# TO-DO 
# [ ] Verificar se arquivo é válido em create_tmp_dot
# [ ] Remover arquivos repetidos (steps repetidos)
# [ ] Fazer para LALR1

count = 0

# Dot files processing
def create_tmp_dots(file_name, tmp_dir):
    write = True
    step=0
    while(write):
        step_str = str(step)
        write = False

        with open(file_name) as f:
            for line in f:
                if(search ("step=" + str(step+1), line.strip())):
                    break

                if ("step=" + step_str) in line:
                    f1 = open(str(tmp_dir) + "/LR-steps-" + step_str + ".dot", "w")
                    write=True
                
                if(write):
                    f1.write(line) 
        step = step + 1 

# Create step by step temporary images from partial dot files
def create_images(tmp_dir):
    id = 0
    for file in os.listdir(tmp_dir):
        convert_line = "dot -Tpng " + tmp_dir + "/LR-steps-" + str(id) + ".dot -o " + tmp_dir + "/" + str(id) + "-out.png"
        os.system(convert_line)
        id = id + 1

# Tkinter canva
class MainCanva(tk.Tk):
    
    def __init__(self, *args, **kwargs):

        tk.Tk.__init__(self, *args, **kwargs)
        self.title('EllErre - Animation for LR0 and LR1')
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

            # Create dot files and images
            create_tmp_dots(file_name, self.tmp_dir.name)
            create_images(self.tmp_dir.name)

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
    
    def load_dot(self):
        global count, img
        file_name =  filedialog.askopenfilename(initialdir = ".",title = "Select file", filetypes=[("dot files","*.dot")])

        # Activate buttons and disable "previous" if set
        self.menu.entryconfig(2, state=tk.DISABLED)
        self.menu.entryconfig(3, state=tk.NORMAL)
        self.menu.entryconfig(4, state=tk.NORMAL)
        count = 0

        self.tmp_dir = tempfile.TemporaryDirectory(dir = ".")
        create_tmp_dots(file_name, self.tmp_dir.name)
        create_images(self.tmp_dir.name)
        self.first_image = self.tmp_dir.name + "/0-out.png"

        img = ImageTk.PhotoImage(Image.open(self.first_image))
        self.myimg = self.canvas.create_image((0, 0), image=img, anchor="nw")
        self.canvas.itemconfig(self.myimg, image = img)

    def close_app(self):
        self.destroy()


if __name__ == "__main__":
    main_canva = MainCanva()