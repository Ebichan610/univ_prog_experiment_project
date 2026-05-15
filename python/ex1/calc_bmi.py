import tkinter as tk

def calc_bmi():
    try:
        height = float(entry_height.get()) / 100
        weight = float(entry_weight.get())
        bmi = weight / (height ** 2)
        result_label.config(text=f"BMI: {bmi:.1f}")
    except ValueError:
        result_label.config(text="数値を入力してください")

root = tk.Tk()
root.title("2412747 PythonでBMI計算")
root.geometry("300x150")
tk.Label(root, text="身長 (cm)").pack()
entry_height = tk.Entry(root)
entry_height.pack()

tk.Label(root, text="体重 (kg)").pack()
entry_weight = tk.Entry(root)
entry_weight.pack()

tk.Button(root, text="計算", command=calc_bmi).pack(pady=10)

result_label = tk.Label(root, text="BMI: -")
result_label.pack()
root.mainloop()
