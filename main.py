import torch
import cv2
import serial
from ultralytics import YOLO

arduino = serial.Serial('COM3', 9600)
print("Arduino connected:", arduino.name)

model_path = "runs/detect/train/weights/best.pt"
model = YOLO(model_path)

cap = cv2.VideoCapture(0)

class_names = class_names = model.names  # Ambil nama kelas langsung dari model

def send_to_arduino(message):
    """Kirim pesan ke Arduino."""
    arduino.write((message + '\n').encode())

print("Starting detection...")
while True:
    ret, frame = cap.read()
    if not ret:
        print("Gagal membaca frame dari webcam.")
        break

    results = model(frame)

    detected_classes = set()  # Set untuk menyimpan kelas yang terdeteksi

    for result in results:
        for box, class_id in zip(result.boxes.xyxy, result.boxes.cls):
            label = class_names[int(class_id)]
            detected_classes.add(label)

    # Logika deteksi
    if "Helmet" in detected_classes and "Vest" in detected_classes:
        send_to_arduino("COMPLETE")  # Atribut lengkap
    elif "Helmet" in detected_classes:
        send_to_arduino("NO_VEST")  # Hanya pakai helmet
    elif "Vest" in detected_classes:
        send_to_arduino("NO_HELMET")  # Hanya pakai vest
    else:
        send_to_arduino("NO_PERSON")  # Tidak ada atribut

    # Tampilkan frame dengan hasil deteksi
    cv2.imshow("YOLOv5 Real-Time Detection", frame)

    # Tekan 'q' untuk keluar
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Lepaskan sumber daya
cap.release()
cv2.destroyAllWindows()
arduino.close()
