import tkinter as tk
from tkinter import filedialog, messagebox

class TextIDE(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title("ASMRed")
        self.geometry("800x600")

        self.toolbar = tk.Frame(self, width=50, height=768)
        self.toolbar.pack(side="left", fill="y")

        self.label = tk.Label(self.toolbar, text="ASMRed", font=("Arial", 15))
        self.label.pack(pady=10)

        self.open_button = tk.Button(self.toolbar, text="Open", command=self.open_file)
        self.open_button.pack(fill="x", pady=10)

        self.save_button = tk.Button(self.toolbar, text="Save", command=self.save_file)
        self.save_button.pack(fill="x", pady=10)

        self.interpret_button = tk.Button(self.toolbar, text="Build BIN", command=self.interpret_code)
        self.interpret_button.pack(fill="x", pady=10)
        
        self.interpret_button = tk.Button(self.toolbar, text="Build ASM", command=self.interpret_code)
        self.interpret_button.pack(fill="x", pady=10)

        self.quit_button = tk.Button(self.toolbar, text="Quit", command=self.quit_app)
        self.quit_button.pack(fill="x", pady=10)

        self.text_area = tk.Text(self, font=("Consolas", 16))
        self.text_area.pack(fill="both", side=tk.RIGHT, expand=True)

        self.line_number_area = tk.Text(self, width=5, height=400, font=("Consolas", 16))
        self.line_number_area.pack(side=tk.LEFT, fill="y")
        self.text_area.tag_config('keyword', foreground='#f250e7')  # purple
        self.text_area.tag_config('comment', foreground='#8d9ba6')  # dark green


        self.keywords = ['null', 'mov','add', 'cout', 'mova', 'inc']
        self.comments = ['; ']

        self.text_area.bind("<KeyRelease>", self.highlight_syntax_realtime)  
        self.update_line_numbers()
    def highlight_syntax_realtime(self, event):
        self.text_area.tag_remove('keyword', '1.0', 'end')
        self.text_area.tag_remove('string', '1.0', 'end')

        text = self.text_area.get('1.0', 'end-1c')
        lines = text.split('\n')

        for i, line in enumerate(lines, 1):
            words = line.split()
            for j, word in enumerate(words):
                if word in self.keywords:
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('keyword', start, end)


            for comment in self.comments:
                if comment in line:
                    start = f'{i}.{line.index(comment)}'
                    end = f'{i}.end'
                    self.text_area.tag_add('comment', start, end)


    def update_line_numbers(self):
        self.line_number_area.delete("1.0", "end")
        for i in range(1, int(self.text_area.index('end-1c').split('.')[0]) + 1):
            self.line_number_area.insert("end", f"{i}\n")
        self.after(100, self.update_line_numbers)

    def open_file(self):
        file_path = filedialog.askopenfilename(filetypes=[('Assembly', '*.asm'), ('Text file', '.txt')])
        if file_path:
            with open(file_path, 'r') as file:
                code = file.read()
                self.text_area.delete('1.0', tk.END)
                self.text_area.insert(tk.END, code)

    def save_file(self):
        file_path = filedialog.asksaveasfilename(defaultextension=".asm", filetypes=[('Assembly', '*.asm')])
        if file_path:
            with open(file_path, 'w') as file:
                code = self.text_area.get("1.0", tk.END)
                file.write(code)

    def interpret_code(self):
        code = self.text_area.get("1.0", tk.END).strip().splitlines()
        commands = []

        for line in code:
            line = line.strip()
            if line.startswith(";") or not line:
                continue  # Игнорируем комментарии и пустые строки

            parts = line.split()
            command = parts[0]

            if command == "null":
                commands.append("0x00")  # Команда для null
            elif command == "add":
                if len(parts) == 3:  # Проверяем, что есть два аргумента
                    args = parts[1].split(',')
                    if len(args) == 2:
                        commands.append(f"01 {args[0].strip()} {args[1].strip()}")  # Команда для add
            elif command == "sub":
                if len(parts) == 3:
                    args = parts[1].split(',')
                    if len(args) == 2:
                        commands.append(f"02 {args[0].strip()} {args[1].strip()}")  # Команда для sub
            elif command == "inc":
                if len(parts) == 2:
                    commands.append(f"03 {parts[1].strip()}")  # Команда для inc
            elif command == "dec":
                if len(parts) == 2:
                    commands.append(f"04 {parts[1].strip()}")  # Команда для dec
                            
            elif command == "mov":
                if len(parts) == 2:
                    commands.append(f"05 {parts[1].strip()}")  # Команда для mov
            elif command == "mova":
                if len(parts) == 2:
                    commands.append(f"06 {parts[1].strip()}")  # Команда для mova
            elif command == "jmp":
                if len(parts) == 2:
                    commands.append(f"07 {parts[1].strip()}")  # Команда для jmp
            elif command == "jmpa":
                if len(parts) == 2:
                    commands.append(f"08 {parts[1].strip()}")  # Команда для jmpa
            elif command == "cout":
                if len(parts) == 2:
                    commands.append(f"09 {parts[1].strip()}")  # Команда для cout
        
        # Сохранение интерпретированных команд в файл
        output_file_path = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[('Text Files', '*.txt')])
        if output_file_path:
            with open(output_file_path, 'w') as output_file:
                for command in commands:
                    output_file.write(command + "\n")
            messagebox.showinfo("Success", "Commands interpreted and saved successfully!")

    def quit_app(self):
        self.destroy()

if __name__ == "__main__":
    app = TextIDE()
    app.mainloop()