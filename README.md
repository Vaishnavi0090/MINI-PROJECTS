# README FILE

## Library Management System

A console-based library management system developed in C++ that allows users to manage books,  
user accounts, and borrowing transactions with role-based access control.

---

## Features

### User Roles

**Students:**
- Borrow up to 3 books for 15 days.
- ₹10/day fine for overdue books.
- View borrowed books and history.

**Faculty:**
- Borrow up to 5 books for 30 days.
- No fines, but blocked if books are 60+ days overdue.

**Librarians:**
- Add/remove/update books.
- Manage user accounts (add/remove users).
- View all transactions and user details.

---

## Core Functionalities

**Book Management:**
- Add, remove, update book details (title, author, ISBN, etc.).
- Track book status (Available/Borrowed/Reserved).

**User Management:**
- Secure login with email/password.
- Role-based permissions (Student/Faculty/Librarian).

**Transactions:**
- Borrow/return books with automatic due date calculation.
- Fine calculation for late returns (Students only).
- View transaction history.

**Data Persistence:**
- All data saved to text files (books.txt, users.txt, etc.).
- Auto-loads previous data on startup.

---

## Getting Started

### Prerequisites:
- C++ compiler (C++11 or later recommended).

---

## Usage

### Login
- Use Login from the main menu. Enter email and password.

### Student/Faculty Menu:
1. View Available Books  
2. Borrow a Book  
3. Return a Book  
4. View Borrowed Books  
5. Borrowing History  
6. Account Details  
7. Pay Fine

### Librarian Menu:
1. Manage Books  
2. Manage Users  
3. View Transactions  
4. View User Accounts

---

## Class Structure

- **Book**: Stores book metadata and status.  
- **User (Base class)**:  
  - *Student*: Handles borrowing rules and fines.  
  - *Faculty*: Custom policies without fines.  
  - *Librarian*: Full access.  
- **Transaction**: Tracks borrow/return dates, fines, and associations.  
- **Date**: Handles due date calculations.  
- **Account**: Tracks user transactions and fines.  
- **Library**: Main logic and data handling.

---

## Data Persistence

Stored in four text files:
- `books.txt`: Book details and statuses.  
- `users.txt`: User credentials and roles.  
- `accounts.txt`: Fines, transaction counts.  
- `transactions.txt`: Borrow/return records.

---

## Notes

- Dates are system-generated (modifiable in code).  
- Fines apply only to students.  
- Faculty lose privileges if any book is 60+ days overdue.
