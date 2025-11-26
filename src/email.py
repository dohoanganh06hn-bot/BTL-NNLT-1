import smtplib
from email.mime.text import MIMEText
import time
import sqlite3
from datetime import datetime, timedelta

MAIL_ADDRESS = "your_email@gmail.com"
MAIL_PASSWORD = "app_password"

def send_email(to_email, subject, body):
    msg = MIMEText(body)
    msg["Subject"] = subject
    msg["From"] = MAIL_ADDRESS
    msg["To"] = to_email

    with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
        server.login(MAIL_ADDRESS, MAIL_PASSWORD)
        server.send_message(msg)
        print(f"Đã gửi mail tới {to_email}")

def get_student_email(student_id):
    conn = sqlite3.connect("db/library.db")
    cursor = conn.cursor()
    cursor.execute("SELECT email, ten FROM sinhvien WHERE id = ?", (student_id,))
    row = cursor.fetchone()
    conn.close()
    if row:
        return row[0], row[1]
    return None, None

def main():
    processed_lines = set()
    reminded_books = set()  # Để tránh nhắc trùng

    while True:
        try:
            # 1️⃣ Gửi email từ log (borrow / reserved)
            try:
                with open("mail_log.txt", "r") as f:
                    lines = f.readlines()

                for line in lines:
                    if line in processed_lines:
                        continue
                    processed_lines.add(line)

                    parts = line.strip().split(",")
                    if len(parts) < 5:
                        continue
                    student_id, book_id, borrow_date, due_date, action = parts
                    student_id = int(student_id)
                    book_id = int(book_id)

                    email, ten = get_student_email(student_id)
                    if email is None:
                        continue

                    if action == "borrow":
                        subject = "Thông báo mượn sách thành công"
                        body = f"Xin chào {ten},\n\nBạn đã mượn sách ID {book_id} từ {borrow_date}.\nHạn trả: {due_date}.\nChúc bạn đọc vui vẻ!"
                    elif action == "reserved":
                        subject = "Thông báo đặt trước sách"
                        body = f"Xin chào {ten},\n\nBạn đã đặt trước sách ID {book_id} vào ngày {borrow_date}.\nKhi sách có sẵn, bạn sẽ nhận thông báo."
                    else:
                        continue

                    send_email(email, subject, body)
            except FileNotFoundError:
                pass

            # 2️⃣ Kiểm tra sách sắp đến hạn trả (1 ngày trước)
            conn = sqlite3.connect("db/library.db")
            cursor = conn.cursor()
            today = datetime.today().date()
            reminder_day = today + timedelta(days=1)

            cursor.execute("""
                SELECT student_id, book_id, due_date
                FROM muonsach
                WHERE status = 'borrowing';
            """)
            rows = cursor.fetchall()
            for student_id, book_id, due_date_str in rows:
                due_date = datetime.strptime(due_date_str, "%Y-%m-%d").date()
                if due_date == reminder_day:
                    key = (student_id, book_id)
                    if key in reminded_books:
                        continue
                    reminded_books.add(key)

                    email, ten = get_student_email(student_id)
                    if email is None:
                        continue

                    subject = "Nhắc nhở trả sách"
                    body = f"Xin chào {ten},\n\nSách ID {book_id} của bạn sẽ đến hạn trả vào ngày {due_date_str}.\nVui lòng trả sách đúng hạn để tránh bị phạt."
                    send_email(email, subject, body)

            conn.close()

        except Exception as e:
            print("Lỗi:", e)

        time.sleep(60)  # kiểm tra mỗi phút

if __name__ == "__main__":
    main()
