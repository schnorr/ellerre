# from PIL import Image, ImageShow, ImageTk
import tkinter as tk
import os,sys
from re import search
import tempfile

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


def main():

    # Create temporary images
    file_name = sys.argv[1]
    tmp_dir = tempfile.TemporaryDirectory(dir = ".")

    create_tmp_dots(file_name, tmp_dir.name)
    create_images(tmp_dir.name)

    # Create animation
    root = tk.Tk()

    # Load first image
    first_image = tmp_dir.name + "/0-out.png"
    img = tk.PhotoImage(file=first_image)

    # Load canva layout images
    button_img_next = tk.PhotoImage(file='./img/next.png')
    button_img_reset = tk.PhotoImage(file='./img/reset.png')

    # Create canva
    canvas = tk.Canvas(root, bg='white')
    # Need to define it based on the maximum size of the image
    canvas.config(width=1000, height=600)
    canvas.pack()
    myimg = canvas.create_image(0, 0, image=img, anchor="nw")

    def on_click_next():
        global count
        global img
        count = count + 1
        filename = tmp_dir.name + '/' + str(count) + '-out.png'
        print(filename)
        # Check if thereis a next image, if not disable button
        if(os.path.exists(filename)):
            img = tk.PhotoImage(file=filename)
        else:
            print("File not accessible")
        canvas.itemconfig(myimg, image = img)

    def on_click_reset():
        global count
        canvas.delete(myimg)
        button1["state"] = "active"
        count=0

    # Load and add buttons to canva
    button1 = tk.Button(root, image=button_img_next, command=on_click_next)
    button1.config(width=1000, height=50)
    button1.pack()

    button2 = tk.Button(root, image=button_img_reset, command=on_click_reset)
    button2.config(width=1000, height=30)
    button2.pack()

    # Open the canva
    root.mainloop()

    tmp_dir.cleanup()


if __name__ == "__main__":
    main()