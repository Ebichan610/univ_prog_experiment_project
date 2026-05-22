import tkinter as tk

def calc_bmi():
    try:
        height = float(entry_height.get()) / 100
        weight = float(entry_weight.get())
        bmi = weight / (height ** 2)
        if(bmi < 18.5):
            result_entry.delete(0, 'end')
            result_entry.insert(0,"やせ型")
        elif(bmi < 25):
            result_entry.delete(0, 'end')
            result_entry.insert(0,"普通")
        elif(bmi < 30):
            result_entry.delete(0, 'end')
            result_entry.insert(0,"肥満")
        else:
            result_entry.delete(0, 'end')
            result_entry.insert(0,"高度肥満")
    except ValueError:
        result_entry.delete(0, 'end')
        result_entry.insert(0,"数値を入力してください")

root = tk.Tk()
root.title("2412747 PythonでBMI計算")
root.geometry("300x150")
tk.Label(root, text="身長(cm)").place(x = 20, y = 20)
entry_height = tk.Entry(root)
entry_height.configure(width = 10)
entry_height.place(x = 20, y = 40)
tk.Label(root, text="体重(kg)").place(x = 140, y = 20)
entry_weight = tk.Entry(root)
entry_weight.configure(width = 10)
entry_weight.place(x = 140, y = 40)

result_entry = tk.Entry(root)
result_entry.configure(width = 15)
result_entry.place(x = 20, y = 80)
tk.Button(root, text="BMI計算", command=calc_bmi).place(x = 160, y = 80)

root.mainloop()
