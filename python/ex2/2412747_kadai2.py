import tkinter as tk

def calc_exp_profit():
    try:
        capital = float(entry_capital.get())
        rate = float(entry_rate.get())
        year = int(entry_year.get())
        for i in range(year):
            capital += capital * (rate / 100)
            i += 1
        entry_result.delete(0, 'end')
        entry_result.insert(0, (int)(capital))
    except ValueError:
        entry_result.delete(0, 'end')
        entry_result.insert(0, "数値を入力してください")
    
root = tk.Tk()
root.title("2412747 Pythonで利率計算")
root.geometry("300x200")
tk.Label(root, text="元本(円)").place(x=20,y=20)
entry_capital = tk.Entry(root)
entry_capital.configure(width = 15)
entry_capital.place(x=100,y=20)
tk.Label(root,text="利率(%)").place(x=20,y=50)
entry_rate = tk.Entry(root)
entry_rate.configure(width=15)
entry_rate.place(x=100,y=50)
entry_year = tk.Entry(root)
entry_year.configure(width = 10)
entry_year.place(x=20,y=80)
tk.Label(root, text="年後の金額(円)").place(x=120,y=80)
entry_result = tk.Entry(root)
entry_result.configure(width=15)
entry_result.place(x=20,y=120)
tk.Button(root, text="計算",command=calc_exp_profit).place(x=160, y=120)
tk.Label(root, text="円です").place(x=80,y=150)

root.mainloop()
