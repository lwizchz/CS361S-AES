#!/usr/bin/env python3

import tkinter as tk
import tkinter.filedialog as tkfile
import tkinter.messagebox as tkmsg
import subprocess

KEY_BYTES_128 = 32
KEY_BYTES_256 = 64

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)

        self.keysize = tk.StringVar()
        self.keysize.set("128")
        self.mode = tk.StringVar()
        self.mode.set("encrypt")
        self.keyfile = tk.StringVar()
        self.inputfile = tk.StringVar()
        self.outputfile = tk.StringVar()

        self.pack()
        self.create_widgets()

        self.master.title("AES")
        self.master.resizable(False, False)

        self.process_update()
    def create_widgets(self):
        self.keysize_frame = tk.Frame(self)
        keysize_label = tk.Label(self.keysize_frame, text="Keysize:")
        keysize_label.pack(side="left")
        keysize_choice_128 = tk.Radiobutton(self.keysize_frame, text="128bit", variable=self.keysize, value="128")
        keysize_choice_128.pack(side="left")
        keysize_choice_256 = tk.Radiobutton(self.keysize_frame, text="256bit", variable=self.keysize, value="256")
        keysize_choice_256.pack(side="left")
        self.keysize_frame.pack()

        self.mode_frame = tk.Frame(self)
        mode_label = tk.Label(self.mode_frame, text="Mode:")
        mode_label.pack(side="left")
        mode_choice_encrypt = tk.Radiobutton(self.mode_frame, text="encrypt", variable=self.mode, value="encrypt")
        mode_choice_encrypt.pack(side="left")
        mode_choice_decrypt = tk.Radiobutton(self.mode_frame, text="decrypt", variable=self.mode, value="decrypt")
        mode_choice_decrypt.pack(side="left")
        self.mode_frame.pack()

        self.keyfile_frame = tk.Frame(self)
        keyfile_label = tk.Label(self.keyfile_frame, text="Keyfile:")
        keyfile_label.pack(side="left")
        keyfile_button = tk.Button(self.keyfile_frame, text="Choose file", command=self.choose_keyfile)
        keyfile_button.pack(side="left")
        keyfile_label_or = tk.Label(self.keyfile_frame, text="or hex:")
        keyfile_label_or.pack(side="left")
        self.keyfile_input = tk.Text(self.keyfile_frame, height=1, width=32)
        self.keyfile_input.pack(side="left")
        self.keyfile_frame.pack()

        self.io_frame = tk.Frame(self)

        self.input_frame = tk.Frame(self.io_frame)
        input_top_frame = tk.Frame(self.input_frame)
        input_label = tk.Label(input_top_frame, text="Input:")
        input_label.pack(side="left")
        input_button = tk.Button(input_top_frame, text="Choose file", command=self.choose_inputfile)
        input_button.pack(side="left")
        input_label_or = tk.Label(input_top_frame, text="or type:")
        input_label_or.pack(side="left")
        input_top_frame.pack(side="top")
        self.input_input = tk.Text(self.input_frame, height=15, width=40)
        self.input_input.pack(side="top")
        self.input_frame.pack(side="left")

        self.output_frame = tk.Frame(self.io_frame)
        output_top_frame = tk.Frame(self.output_frame)
        output_label = tk.Label(output_top_frame, text="Output:")
        output_label.pack(side="left")
        output_button = tk.Button(output_top_frame, text="Choose file", command=self.choose_outputfile)
        output_button.pack(side="left")
        output_label_or = tk.Label(output_top_frame, text="as hex:")
        output_label_or.pack(side="left")
        output_top_frame.pack(side="top")
        self.output_input = tk.Text(self.output_frame, height=15, width=40)
        self.output_input.pack(side="top")
        self.output_frame.pack(side="right")

        self.io_frame.pack()

        self.process_frame = tk.Frame(self)
        process_button = tk.Button(self.process_frame, text="Process", command=self.process)
        process_button.pack()
        self.process_frame.pack()

    def choose_keyfile(self):
        f = tkfile.askopenfilename()
        if f:
            self.keyfile_input.delete("1.0", tk.END)
            self.keyfile.set("@"+f)
    def choose_inputfile(self):
        f = tkfile.askopenfilename()
        if f:
            self.input_input.delete("1.0", tk.END)
            self.inputfile.set("@"+f)
    def choose_outputfile(self):
        f = tkfile.askopenfilename()
        if f:
            self.output_input.delete("1.0", tk.END)
            self.outputfile.set("@"+f)
    def process(self):
        kfi = self.keyfile_input.get("1.0", "end-1c")
        if kfi and self.keyfile.get() != kfi:
            self.keyfile.set(kfi)
        ifi = self.input_input.get("1.0", "end-1c")
        if ifi and self.inputfile.get() != ifi:
            self.inputfile.set(ifi)

        keyfile = self.keyfile.get()
        if keyfile and keyfile[0] == '@':
            keyfile = keyfile[1:]
        else:
            kfi = keyfile
            keyfile = "/tmp/aes.keyfile"
            with open(keyfile, "wb") as f:
                f.write(bytes.fromhex(kfi))
        inputfile = self.inputfile.get()
        if inputfile and inputfile[0] == '@':
            inputfile = inputfile[1:]
        else:
            ifi = inputfile
            inputfile = "/tmp/aes.inputfile"
            with open(inputfile, "wb") as f:
                if self.mode.get() == "encrypt":
                    f.write(ifi.encode("utf8"))
                else:
                    f.write(bytes.fromhex(ifi))
        outputfile = self.outputfile.get()
        if outputfile and outputfile[0] == '@':
            outputfile[0] == outputfile[1:]
        else:
            outputfile = "/tmp/aes.outputfile"

        if keyfile and inputfile and outputfile:
            cmd = ["./aes", "--keysize", self.keysize.get(), "--mode", self.mode.get(), "--keyfile", keyfile, "--inputfile", inputfile, "--outputfile", outputfile]
            print(" ".join(cmd))

            try:
                subprocess.run(cmd, check=True)
            except FileNotFoundError as e:
                print(e)
                tkmsg.showwarning("AES", "Failed to run ./aes, make sure it's been compiled with `make`")
                return

            with open(outputfile, "rb") as f:
                self.output_input.delete("1.0", tk.END)
                if self.mode.get() == "encrypt":
                    self.output_input.insert(tk.END, f.read().hex())
                else:
                    try:
                        b = f.read()
                        self.output_input.insert(tk.END, b.decode("utf8"))
                    except UnicodeDecodeError as e:
                        print(e)
                        tkmsg.showwarning("AES", "Failed to decode: {}".format(b))
        else:
            msg = "Invalid argument"
            if not keyfile:
                msg = "Invalid keyfile"
            elif not inputfile:
                msg = "Invalid input file"
            elif not outputfile:
                msg = "Invalid output file"
            tkmsg.showwarning("AES", msg)
    def process_update(self):
        should_process = False

        kfi = self.keyfile_input.get("1.0", "end-1c")
        if kfi and self.keyfile.get() != kfi:
            self.keyfile.set(kfi)
            should_process = True
        ifi = self.input_input.get("1.0", "end-1c")
        if ifi and self.inputfile.get() != ifi:
            self.inputfile.set(ifi)
            should_process = True

        if should_process and self.keyfile.get() and self.inputfile.get():
            kf = self.keyfile.get()
            b = KEY_BYTES_128 if self.keysize.get() == "128" else KEY_BYTES_256
            if (len(kf) == b and kf[0] != '@') or (len(kf) and kf[0] == '@'):
                if self.inputfile.get()[0] != '@':
                    self.process()

        self.master.after(500, self.process_update)

if __name__ == "__main__":
    root = tk.Tk()
    app = Application(master=root)
    app.mainloop()
