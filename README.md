<div align="center">
  <h1>🛒 Secure E-Commerce Shop (C++)</h1>
  <p>A comprehensive terminal-based E-Commerce management system built entirely in standard C++.</p>

  [![Language: C++](https://img.shields.io/badge/Language-C++-blue.svg)]()
  [![Build: Make](https://img.shields.io/badge/Build-Make-green.svg)]()
  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
</div>

---

> [!NOTE]
> **Academic Project**
> This repository showcases a robust C++ programming project designed under strict constraints (e.g., utilizing foundational C-style arrays, basic I/O, and manual memory/string handling) while still delivering complex features like role-based authentication and persistent storage.

## 📖 Table of Contents
- [About the Project](#about-the-project)
- [Key Features](#key-features)
- [Roles & Permissions](#roles--permissions)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [License](#license)

## 🕵️‍♂️ About the Project

The **Secure E-Commerce Shop** is a fully functional console application that simulates a digital storefront. It allows different types of users (Admins, Employees, and Customers) to interact with a centralized product inventory. 

The system relies on file I/O to persist data, meaning products, user credentials, failed login attempts, and sales records are saved across sessions.

## ✨ Key Features

- **Custom Authentication**: Includes registration and login mechanisms with role-based access control. Logs failed attempts for security auditing.
- **Admin OTP System**: Extra layer of security requiring a One-Time Password for Administrator access.
- **Inventory Management**: Admins and Employees can add, remove, and update product details (Price, Category, Quantity).
- **Shopping Cart & Wishlist**: Customers can browse products, add them to a wishlist, or place them in a cart for checkout.
- **Discounts & Promo Codes**: Built-in support for product-specific discounts and promotional codes during checkout.
- **Persistent Storage**: Reads from and writes to `.txt` files seamlessly to maintain state.

## 👥 Roles & Permissions

1. **Admin (`admin`)**
   - Requires an OTP to log in.
   - Can manage inventory (add/remove products).
   - Can manage promo codes and discounts.
   - Can view global sales records and failed login attempts.
   - Can view all registered users.

2. **Employee (`employee`)**
   - Can view the product inventory.
   - Can manage basic product details (add/remove products).

3. **Customer (`customer`)**
   - Can view products and categories.
   - Can manage a personal Shopping Cart (add, remove, checkout).
   - Can manage a personal Wishlist.

## 🚀 Getting Started

### Prerequisites
You need a standard C++ compiler (like `g++`) installed on your machine.
- **Windows**: Install MinGW-w64.
- **Linux/Mac**: `sudo apt install build-essential` or install Xcode Command Line Tools.

### Build Instructions
Clone the repository and compile using the included `Makefile`:

```bash
git clone https://github.com/your-username/Secure-Shop-CPP.git
cd Secure-Shop-CPP
make
```

## 🎮 Usage

Run the compiled executable:

```bash
# On Linux/Mac:
./secure_shop

# On Windows:
.\secure_shop.exe
```

1. **First Run**: Select `1. Register` to create an `admin` account.
2. **Login**: Select `2. Login` and enter the credentials you just created.
3. **Explore**: Navigate the menus to add products, then log out and create a `customer` account to buy them!

## 📝 License

Distributed under the MIT License. See `LICENSE` for more information.
