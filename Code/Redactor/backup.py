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
        self.interpret_button = tk.Button(self.toolbar, text="Interpret", command=self.interpret_code)
        self.interpret_button.pack(fill="x", pady=10)
        self.quit_button = tk.Button(self.toolbar, text="Quit", command=self.quit_app)
        self.quit_button.pack(fill="x", pady=10)
        self.text_area = tk.Text(self, font=("Consolas", 12))
        self.text_area.pack(fill="both", side=tk.RIGHT, expand=True)
        self.line_number_area = tk.Text(self, width=5, height=400, font=("Consolas", 12))
        self.line_number_area.pack(side=tk.LEFT, fill="y")
        self.update_line_numbers()
    def update_line_numbers(self):
        self.line_number_area.delete("1.0", "end")
        for i in range(1, int(self.text_area.index('end-1c').split('.')[0]) + 1):
            self.line_number_area.insert("end", f"{i}\n")
        self.after(100, self.update_line_numbers)
    def open_file(self):
        file_path = filedialog.askopenfilename(filetypes=[('Assembly', '*.asm')])
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