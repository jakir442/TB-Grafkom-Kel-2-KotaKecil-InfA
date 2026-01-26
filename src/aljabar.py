import tkinter as tk
from tkinter import filedialog
import math
import csv

DERAJAT_KE_KM = 111  # 1 derajat ≈ 111 km

# ===============================
# DATA
# ===============================
titik_latlon = []

# ===============================
# TAMBAH MANUAL (BATAS WILAYAH)
# ===============================
def tambah_titik():
    try:
        lat = float(entry_lat.get())
        lon = float(entry_lon.get())
        titik_latlon.append((lat, lon))
        listbox.insert(tk.END, f"{len(titik_latlon)}. {lat}, {lon}")
        entry_lat.delete(0, tk.END)
        entry_lon.delete(0, tk.END)
    except:
        output.insert(tk.END, "❌ Input latitude / longitude tidak valid\n")

# ===============================
# UPLOAD FILE
# ===============================
def upload_file():
    titik_latlon.clear()
    listbox.delete(0, tk.END)
    output.delete(1.0, tk.END)

    path = filedialog.askopenfilename(
        filetypes=[("Text File", "*.txt"), ("CSV File", "*.csv")]
    )
    if not path:
        return

    try:
        if path.endswith(".txt"):
            with open(path) as f:
                for line in f:
                    lat, lon = map(float, line.split())
                    titik_latlon.append((lat, lon))

        elif path.endswith(".csv"):
            with open(path, newline='') as f:
                reader = csv.DictReader(f)
                for r in reader:
                    titik_latlon.append(
                        (float(r["latitude"]), float(r["longitude"]))
                    )

        for i, p in enumerate(titik_latlon):
            listbox.insert(tk.END, f"{i+1}. {p[0]}, {p[1]}")

        output.insert(
            tk.END,
            f"✅ {len(titik_latlon)} titik batas wilayah dimuat\n"
        )

    except Exception as e:
        output.insert(tk.END, f"❌ Error file: {e}\n")

# ===============================
# LUAS POLYGON (SHOELACE)
# ===============================
def luas_polygon_shoelace(titik):
    luas = 0
    n = len(titik)
    for i in range(n):
        x1, y1 = titik[i]
        x2, y2 = titik[(i + 1) % n]
        luas += (x1 * y2) - (x2 * y1)
    return abs(luas) / 2

# ===============================
# HITUNG LUAS
# ===============================
def hitung_luas():
    output.insert(tk.END, "\n" + "="*60 + "\n")

    n = len(titik_latlon)
    if n < 3:
        output.insert(tk.END, "❌ Minimal 3 titik batas wilayah\n")
        return

    # ===============================
    # TITIK PUSAT (CENTROID)
    # ===============================
    lat0 = sum(p[0] for p in titik_latlon) / n
    lon0 = sum(p[1] for p in titik_latlon) / n
    lat0_rad = math.radians(lat0)

    SCALE = 1.0421 #...

    # ===============================
    # PROYEKSI DERAJAT → KM
    # ===============================
    def ke_km(lat, lon):
        x = (lon - lon0) * DERAJAT_KE_KM * math.cos(lat0_rad) * SCALE
        y = (lat - lat0) * DERAJAT_KE_KM * SCALE
        return (x, y)

    titik_km = [ke_km(lat, lon) for lat, lon in titik_latlon]

    # ===============================
    # TITIK EKSTREM
    # ===============================
    barat = min(titik_km, key=lambda p: p[0])
    timur = max(titik_km, key=lambda p: p[0])
    selatan = min(titik_km, key=lambda p: p[1])
    utara = max(titik_km, key=lambda p: p[1])

    # ===============================
    # HITUNG LUAS
    # ===============================
    total = luas_polygon_shoelace(titik_km)

    # ===============================
    # OUTPUT
    # ===============================
    output.insert(tk.END, "TITIK EKSTREM WILAYAH\n")
    output.insert(tk.END, "-"*30 + "\n")
    output.insert(tk.END, f"Barat   : {barat}\n")
    output.insert(tk.END, f"Timur   : {timur}\n")
    output.insert(tk.END, f"Selatan : {selatan}\n")
    output.insert(tk.END, f"Utara   : {utara}\n\n")

    output.insert(tk.END, "TITIK PUSAT (DERAJAT)\n")
    output.insert(tk.END, "-"*30 + "\n")
    output.insert(tk.END, f"Latitude  : {lat0:.6f}\n")
    output.insert(tk.END, f"Longitude : {lon0:.6f}\n\n")\

    output.insert(tk.END, "HASIL PERHITUNGAN\n")
    output.insert(tk.END, "-"*30 + "\n")
    output.insert(tk.END, f"Jumlah Titik        : {n}\n")
    output.insert(tk.END, f"Jumlah Segitiga     : {n}\n")
    output.insert(tk.END, f"Luas Total Wilayah  : {total:.2f} km²\n")

# ===============================
# GUI
# ===============================
root = tk.Tk()
root.title("Hitung Luas Wilayah - Metode Segitiga")
root.geometry("950x750")

tk.Label(
    root,
    text="Input Titik Batas Wilayah (Latitude Longitude)",
    font=("Arial", 12, "bold")
).pack()

frame = tk.Frame(root)
frame.pack(pady=5)

entry_lat = tk.Entry(frame, width=10)
entry_lon = tk.Entry(frame, width=10)
entry_lat.grid(row=0, column=0)
entry_lon.grid(row=0, column=1)

tk.Label(frame, text="Latitude").grid(row=1, column=0)
tk.Label(frame, text="Longitude").grid(row=1, column=1)

tk.Button(frame, text="Tambah Titik", command=tambah_titik)\
    .grid(row=0, column=2, padx=5)

tk.Button(
    root,
    text="Upload File Koordinat",
    command=upload_file,
    bg="lightgreen"
).pack(pady=8)

listbox = tk.Listbox(root, width=60, height=10)
listbox.pack()

tk.Button(
    root,
    text="Hitung Luas Wilayah",
    command=hitung_luas,
    bg="lightblue",
    font=("Arial", 12, "bold")
).pack(pady=10)

output = tk.Text(root, height=18, width=110)
output.pack()

root.mainloop()
