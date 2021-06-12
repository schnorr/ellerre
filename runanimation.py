# from PIL import Image, ImageShow, ImageTk
import tkinter as tk
import os,sys
from re import search
import tempfile
from PIL import Image, ImageTk
 
count = 0

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


def create_images(tmp_dir):
    id = 0
    for file in os.listdir(tmp_dir):
        convert_line = "dot -Tpng " + tmp_dir + "/LR-steps-" + str(id) + ".dot -o " + tmp_dir + "/" + str(id) + "-out.png"
        os.system(convert_line)
        id = id + 1

class MainCanva(tk.Tk):
    
    def __init__(self, *args, **kwargs):

        tk.Tk.__init__(self, *args, **kwargs)
        self.title('EllErre - Animation')

        # Create temporary images
        file_name = sys.argv[1]
        self.tmp_dir = tempfile.TemporaryDirectory(dir = ".")

        create_tmp_dots(file_name, self.tmp_dir.name)
        create_images(self.tmp_dir.name)

        # Create canva
        self.canvas = tk.Canvas(self, bg='white')

        # Need to define it based on the maximum size of the image
        self.canvas.config(width=1100, height=620)
        self.canvas.pack(fill=tk.BOTH, expand=1)
        self.canvas.bind("<Configure>", self.resize)
        self.first_image = self.tmp_dir.name + "/0-out.png"
        self.img = ImageTk.PhotoImage(Image.open(self.first_image))

        # Load first image
        # self.img = tk.PhotoImage(file=self.first_image)
        self.myimg = self.canvas.create_image((0, 0), image=self.img, anchor="nw")

        # Load canva layout images
        button_img_next = tk.PhotoImage(file=r'./img/next.png')
        button_img_reset = tk.PhotoImage(file=r'./img/reset.png')

        # Load and add buttons to canva
        self.button1 = tk.Button(self, image=button_img_next, command=self.on_click_next)
        self.button1.config(width=1100, height=50)
        self.button1.pack()

        button2 = tk.Button(self, image=button_img_reset, command=self.on_click_reset)
        button2.config(width=1100, height=30)
        button2.pack()

        # Open the canva
        self.mainloop()
        # self.tmp_dir.cleanup()

    def on_click_next(self):
        global img, count
        count = count + 1
        filename = self.tmp_dir.name + '/' + str(count) + '-out.png'

        # Check if thereis a next image, if not disable button
        if(os.path.exists(filename)):
            img = tk.PhotoImage(file=filename)
        else:
            self.button1["state"] = "disable"
            print("File not accessible")
        self.canvas.itemconfig(self.myimg, image = img)

    def on_click_reset(self):
        global img, count
        self.button1["state"] = "active"
        img = tk.PhotoImage(file=self.first_image)
        self.canvas.itemconfig(self.myimg, image = img)
        count=0

    def resize(self, event):
        global count
        filename = self.tmp_dir.name + '/' + str(count) + '-out.png'

        img = Image.open(filename).resize(
            (event.width, event.height), Image.ANTIALIAS
        )
        self.img = ImageTk.PhotoImage(img)
        self.canvas.itemconfig(self.myimg, image=self.img)

    def close_app(self):
        self.destroy()


if __name__ == "__main__":
    main_canva = MainCanva()