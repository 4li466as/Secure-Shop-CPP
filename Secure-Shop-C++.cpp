#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

const int MAX_PRODUCTS = 100;

// Arrays for product details
int productIDs[MAX_PRODUCTS];
char productNames[MAX_PRODUCTS][30];
float productPrices[MAX_PRODUCTS];
int productQuantities[MAX_PRODUCTS];
char productCategories[MAX_PRODUCTS][20];
float productDiscounts[MAX_PRODUCTS];
int productCount = 0; // Number of products in inventory

void loadProductsFromFile();
void saveProductsToFile();
void removePromoCode();
void viewPromoCodes();
void viewProductDiscounts();
void removeProductDiscount();

// Arrays for cart
int cartIDs[MAX_PRODUCTS];
int cartQuantities[MAX_PRODUCTS];
int cartCount = 0;

// Arrays for wishlist
int wishlistIDs[MAX_PRODUCTS];
int wishlistQuantities[MAX_PRODUCTS];
int wishlistCount = 0;

// Function Prototypes
void registerUser();
bool loginUser(char* retrievedRole);
bool adminOTP();
void adminMenu();
void customerMenu();
void employeeMenu();
void viewAllUsers();
void viewProducts();
void addProduct();
void removeProduct();
void addToCart();
void removeFromCart();
void checkout();
void viewSales();
void addToWishlist();
void removeFromWishlist();
void viewWishlist();
bool stringsEqual(const char* str1, const char* str2);
void stringCopy(char* dest, const char* src);

// Main Function
int main() {
    
    loadProductsFromFile();
     
	int choice;
	char retrievedRole[10];

	while (true) {
		cout << "\nWelcome to Secure E-Commerce Shop\n";
		cout << "1. Register\n";
		cout << "2. Login\n";
		cout << "3. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			registerUser();
			break;
		case 2:
			if (loginUser(retrievedRole)) {
				if (stringsEqual(retrievedRole, "admin")) {
					if (adminOTP()) {
						adminMenu();
					} else {
						cout << "Invalid OTP. Access denied.\n";
					}
				} else if (stringsEqual(retrievedRole, "customer")) {
					customerMenu();
				} else if (stringsEqual(retrievedRole, "employee")) {
					employeeMenu();
				} else {
					cout << "Unknown role. Access denied.\n";
				}
			} else {
				cout << "Login failed. Check your credentials.\n";
			}
			break;
		case 3:
			cout << "Exiting the program. Goodbye!\n";
			exit(0);
		default:
			cout << "Invalid choice. Please try again.\n";
		}
	}

	return 0;
}

// Custom string comparison
bool stringsEqual(const char* str1, const char* str2) {
	while (*str1 && *str2) {
		if (*str1 != *str2) {
			return false;
		}
		++str1;
		++str2;
	}
	return *str1 == '\0' && *str2 == '\0';
}

// Custom string copy
void stringCopy(char* dest, const char* src) {
	while (*src) {
		*dest++ = *src++;
	}
	*dest = '\0';
}

// Register a new user
void registerUser() {
	char username[20], password[20], role[10];
	ofstream usersFile("users.txt", ios::app);

	if (!usersFile) {
		cout << "Error opening file for registration.\n";
		return;
	}

	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;
	cout << "Enter role (admin/customer/employee): ";
	cin >> role;

	usersFile << username << " " << password << " " << role << endl;
	usersFile.close();

	cout << "Registration successful!\n";
}

// Login a user
bool loginUser(char* retrievedRole) {
	char username[20], password[20];
	char fileUsername[20], filePassword[20], fileRole[10];
	ifstream usersFile("users.txt");

	if (!usersFile) {
		cout << "Error opening file for login.\n";
		return false;
	}

	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;

	while (usersFile >> fileUsername >> filePassword >> fileRole) {
		if (stringsEqual(username, fileUsername) && stringsEqual(password, filePassword)) {
			stringCopy(retrievedRole, fileRole);
			usersFile.close();
			return true;
		}
	}

	// Log failed login attempt
	usersFile.close();
	ofstream failedLoginsFile("failed_logins.txt", ios::app);
	if (failedLoginsFile) {
		failedLoginsFile << username << " " << password << " " << time(0) << "\n";
		failedLoginsFile.close();
	}

	return false;
}

//failed attempts saved
void viewFailedLogins() {
	ifstream failedLoginsFile("failed_logins.txt");

	if (!failedLoginsFile) {
		cout << "Error opening failed logins file.\n";
		return;
	}

	char username[20], password[20];
	time_t timestamp;

	cout << "\n--- Failed Login Attempts ---\n";
	cout << left << setw(20) << "Username" << setw(20) << "Password" << "Timestamp\n";
	cout << string(60, '-') << "\n";

	while (failedLoginsFile >> username >> password >> timestamp) {
		// Convert timestamp to readable format
		char timeString[25];
		strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));

		cout << left << setw(20) << username << setw(20) << password << timeString << "\n";
	}

	failedLoginsFile.close();
}

// View all registered users
void viewAllUsers() {
	ifstream usersFile("users.txt");

	if (!usersFile) {
		cout << "Error opening users file.\n";
		return;
	}

	char username[20], password[20], role[10];

	// Print table header
	cout << left << setw(20) << "Username"
	     << "|" << setw(20) << "Password"
	     << "|" << setw(10) << "Role" << "\n";
	cout << string(55, '-') << "\n"; // Horizontal line

	// Print user details
	while (usersFile >> username >> password >> role) {
		cout << left << setw(20) << username
		     << "|" << setw(20) << password
		     << "|" << setw(10) << role << "\n";
	}

	usersFile.close();
}

// Generate and validate admin OTP
bool adminOTP() {
	srand(time(0));
	int generatedOTP = rand() % 9000 + 1000; // Generate a 4-digit OTP
	int enteredOTP;

	cout << "Generated OTP: " << generatedOTP << " (For testing purposes, this would be sent securely)\n";
	cout << "Enter OTP: ";
	cin >> enteredOTP;

	return enteredOTP == generatedOTP;
}

// Add a product to inventory
void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        cout << "Cannot add more products. Inventory is full.\n";
        return;
    }

    int newProductID;
    cout << "Enter product ID: ";
    cin >> newProductID;

    // Check for duplicate ID
    for (int i = 0; i < productCount; i++) {
        if (productIDs[i] == newProductID) {
            cout << "Error: Product ID " << newProductID << " already exists.\n";
            return;
        }
    }

    // Continue adding the product
    productIDs[productCount] = newProductID;
    cout << "Enter product name: ";
    cin >> productNames[productCount];
    cout << "Enter product price: ";
    cin >> productPrices[productCount];
    cout << "Enter product quantity: ";
    cin >> productQuantities[productCount];
    cout << "Enter product category: ";
    cin >> productCategories[productCount];
    productDiscounts[productCount] = 0.0; // Default discount is 0%
    productCount++;

    cout << "Product added successfully.\n";
    saveProductsToFile();
}

// Remove a product from inventory
void removeProduct() {
	int productId;
	cout << "Enter product ID to remove: ";
	cin >> productId;

	bool found = false;
	for (int i = 0; i < productCount; i++) {
		if (productIDs[i] == productId) {
			// Shift all elements after the removed product
			for (int j = i; j < productCount - 1; j++) {
				productIDs[j] = productIDs[j + 1];
				stringCopy(productNames[j], productNames[j + 1]);
				productPrices[j] = productPrices[j + 1];
				productQuantities[j] = productQuantities[j + 1];
				stringCopy(productCategories[j], productCategories[j + 1]);
				productDiscounts[j] = productDiscounts[j + 1];
			}
			productCount--;
			found = true;
			cout << "Product removed successfully.\n";
			break;
		}
	}

	if (!found) {
		cout << "Product not found.\n";
	}
}

//Restock Product
void restockProduct() {
	int productId;
	int restockQuantity;

	cout << "Enter the ID of the product to restock: ";
	cin >> productId;
	cout << "Enter the quantity to add: ";
	cin >> restockQuantity;

	bool found = false;
	for (int i = 0; i < productCount; i++) {
		if (productIDs[i] == productId) {  // Compare by ID
			productQuantities[i] += restockQuantity;
			cout << "Product restocked successfully. New quantity: " << productQuantities[i] << "\n";
			found = true;
			break;
		}
	}

	if (!found) {
		cout << "Product ID not found.\n";
	}
}

// View Products
void viewProducts() {
	if (productCount == 0) {
		cout << "No products available.\n";
		return;
	}

	// Print the headers with Product ID, Name, Price, Quantity, Discount, and Category
	cout << left << setw(10) << "Product ID"  // Added Product ID header
	     << setw(20) << "Name"
	     << setw(10) << "Price"
	     << setw(10) << "Quantity"
	     << setw(10) << "Discount (%)"
	     << setw(20) << "Category" << "\n";  // Added Category header
	cout << string(80, '-') << "\n";  // Line separator

	// Loop through the products and display their details
	for (int i = 0; i < productCount; i++) {
		cout << left << setw(10) << productIDs[i]   // Display Product ID
		     << setw(20) << productNames[i]
		     << setw(10) << productPrices[i]
		     << setw(10) << productQuantities[i]
		     << setw(10) << productDiscounts[i]
		     << setw(20) << productCategories[i] << "\n";  // Display Category
	}
}
//Seperate discount on product
void setProductDiscount() {
    int productId;
    float discountPercentage;
    bool found = false;

    cout << "Enter product ID to apply discount: ";
    cin >> productId;

    // Validate discount percentage input
    while (true) {
        cout << "Enter discount percentage: ";
        cin >> discountPercentage;

        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a numeric value.\n";
        } else if (discountPercentage < 0 || discountPercentage > 100) {
            cout << "Error: Discount percentage must be between 0 and 100.\n";
        } else {
            break; // Valid input
        }
    }

    // Search for the product and apply the discount
    for (int i = 0; i < productCount; i++) {
        if (productIDs[i] == productId) {
            productDiscounts[i] = discountPercentage;
            found = true;
            cout << "Discount applied successfully to product ID " << productId << ".\n";
            break;
        }
    }

    if (!found) {
        cout << "Product ID not found.\n";
    }
}


//Remove Product discount
void removeProductDiscount() {
    int productId;
    bool found = false;

    cout << "Enter the product ID to remove discount: ";
    cin >> productId;

    for (int i = 0; i < productCount; i++) {
        if (productIDs[i] == productId) {
            // Reset the discount for the product
            productDiscounts[i] = 0.0;
            found = true;
            cout << "Discount removed for product ID " << productId << ".\n";
            break;
        }
    }

    if (!found) {
        cout << "Product ID not found.\n";
    } else {
        // Save the updated products to the file
        saveProductsToFile();
    }
}

//View Product Discount
void viewProductDiscounts() {
    if (productCount == 0) {
        cout << "No products available to view.\n";
        return;
    }

    cout << "\n--- Product Discounts ---\n";
    cout << left << setw(10) << "Product ID" 
         << setw(20) << "Product Name" 
         << setw(10) << "Discount (%)" << "\n";
    cout << string(50, '-') << "\n";

    // Loop through the products and display their ID, name, and discount
    for (int i = 0; i < productCount; i++) {
        cout << left << setw(10) << productIDs[i]
             << setw(20) << productNames[i]
             << setw(10) << productDiscounts[i] << "\n";
    }
}


//Promo code maker
void generatePromoCode() {
    char promoCode[20];
    float discountPercentage;

    // Open the promo codes file for reading to check for duplicates
    ifstream promoFileRead("promo_codes.txt");
    if (!promoFileRead) {
        cout << "Error opening promo codes file for reading.\n";
        return;
    }

    cout << "Enter a promotion code: ";
    cin >> promoCode;

    // Check for duplicate promo code
    char existingCode[20];
    float existingDiscount;
    while (promoFileRead >> existingCode >> existingDiscount) {
        if (stringsEqual(existingCode, promoCode)) {
            cout << "Error: Promo code '" << promoCode << "' already exists.\n";
            promoFileRead.close();
            return;
        }
    }
    promoFileRead.close();

    // Input and validate the discount percentage
    while (true) {
        cout << "Enter discount percentage (e.g., 10 for 10%): ";
        cin >> discountPercentage;

        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a numeric value.\n";
        } else if (discountPercentage < 0 || discountPercentage > 100) {
            cout << "Error: Discount percentage must be between 0 and 100.\n";
        } else {
            break; // Valid input
        }
    }

    // Open the promo codes file for appending to add the new promo code
    ofstream promoFileWrite("promo_codes.txt", ios::app);
    if (!promoFileWrite) {
        cout << "Error opening promo codes file for writing.\n";
        return;
    }

    promoFileWrite << promoCode << " " << discountPercentage << "\n";
    promoFileWrite.close();

    cout << "Promotion code created successfully!\n";
}


//Remove Promo code
void removePromoCode() {
    string codeToRemove;
    bool codeFound = false;

    // Open the promo_codes.txt file for reading and a temporary file for writing
    ifstream promoFile("promo_codes.txt");
    ofstream tempFile("temp_promo_codes.txt");

    if (!promoFile) {
        cout << "Error opening promo codes file.\n";
        return;
    }

    // Read the file line by line
    cout << "Enter the promo code to remove: ";
    cin >> codeToRemove;

    string promoCode;
    float discount;
    
    // Process each line
    while (promoFile >> promoCode >> discount) {
        if (promoCode != codeToRemove) {
            // Write the promo code and discount to the temporary file
            tempFile << promoCode << " " << discount << "\n";
        } else {
            // Found the code to remove
            codeFound = true;
        }
    }

    // Close both files
    promoFile.close();
    tempFile.close();

    // Remove the old promo codes file and rename the temp file to the original file
    if (codeFound) {
        remove("promo_codes.txt");
        rename("temp_promo_codes.txt", "promo_codes.txt");
        cout << "Promo code '" << codeToRemove << "' removed successfully.\n";
    } else {
        cout << "Promo code '" << codeToRemove << "' not found.\n";
        remove("temp_promo_codes.txt"); // Remove the temporary file if code was not found
    }
}

//View Promo code
void viewPromoCodes() {
    ifstream promoFile("promo_codes.txt");
    if (!promoFile) {
        cout << "Error opening promo codes file.\n";
        return;
    }

    string promoCode;
    float discount;

    cout << "\n--- Current Promo Codes ---\n";
    while (promoFile >> promoCode >> discount) {
        cout << "Promo Code: " << promoCode << " | Discount: " << discount << "%\n";
    }

    promoFile.close();
}


//Checkout promo code ask
float applyDiscount(float totalAmount) {
	char promoCode[20];
	char filePromoCode[20];
	float discountPercentage;
	bool promoFound = false;

	cout << "Do you have a promotion code? (yes/no): ";
	string response;
	cin >> response;

	if (response == "yes" || response == "Yes") {
		cout << "Enter the promotion code: ";
		cin >> promoCode;

		// Open the promo codes file
		ifstream promoFile("promo_codes.txt");
		if (!promoFile) {
			cout << "Error opening promo codes file.\n";
			return totalAmount;
		}

		// Check if the promo code exists
		while (promoFile >> filePromoCode >> discountPercentage) {
			if (stringsEqual(filePromoCode, promoCode)) {
				promoFound = true;
				break;
			}
		}
		promoFile.close();

		// Apply the discount if the promo code is valid
		if (promoFound) {
			float discountAmount = (totalAmount * discountPercentage) / 100;
			cout << "Promotion code applied! You get a discount of: " << discountAmount << "\n";
			return totalAmount - discountAmount;
		} else {
			cout << "Invalid promotion code.\n";
		}
	}

	return totalAmount;
}

//View Feedback
void viewFeedback() {
    ifstream feedbackFile("feedback.txt");

    if (!feedbackFile) {
        cout << "Error opening feedback file.\n";
        return;
    }

    string customerName, feedbackText, line;
    cout << "\n--- Customer Feedback ---\n";

    while (getline(feedbackFile, line)) {
        // Parse each line to extract the customer name and feedback
        size_t separator = line.find(" | ");
        if (separator != string::npos) {
            customerName = line.substr(10, separator - 10); // Extract customer name (after "Customer: ")
            feedbackText = line.substr(separator + 3);      // Extract feedback text
            cout << "Customer: " << customerName << "\n"
                 << "Feedback: " << feedbackText << "\n"
                 << string(40, '-') << "\n";               // Separator line for clarity
        } else {
            cout << "Malformed feedback entry: " << line << "\n"; // Handle unexpected format
        }
    }

    feedbackFile.close();
}

// Function to view product requests
void viewProductRequests() {
	ifstream requestFile("product_requests.txt");

	if (!requestFile) {
		cout << "Error opening product requests file.\n";
		return;
	}

	string line;
	cout << "\n--- Product Requests ---\n";
	while (getline(requestFile, line)) {
		cout << line << "\n";
	}

	requestFile.close();
}

// Add a product to cart
void addToCart() {
	int productId, quantity;
	cout << "Enter product ID to add to cart: ";
	cin >> productId;
	cout << "Enter quantity: ";
	cin >> quantity;

	bool found = false;
	for (int i = 0; i < productCount; i++) {
		if (productIDs[i] == productId && productQuantities[i] >= quantity) {
			cartIDs[cartCount] = productId;
			cartQuantities[cartCount] = quantity;
			cartCount++;
			productQuantities[i] -= quantity;
			cout << "Product added to cart.\n";
			found = true;
			break;
		}
	}

	if (!found) {
		cout << "Product not found or insufficient quantity.\n";
	}
}

// Remove a product from cart
void removeFromCart() {
	int productId;
	cout << "Enter product ID to remove from cart: ";
	cin >> productId;

	bool found = false;
	for (int i = 0; i < cartCount; i++) {
		if (cartIDs[i] == productId) {
			// Shift all elements after the removed product
			for (int j = i; j < cartCount - 1; j++) {
				cartIDs[j] = cartIDs[j + 1];
				cartQuantities[j] = cartQuantities[j + 1];
			}
			cartCount--;
			found = true;
			cout << "Product removed from cart.\n";
			break;
		}
	}

	if (!found) {
		cout << "Product not found in cart.\n";
	}
}

// View all products in the cart
void viewCart() {
    if (cartCount == 0) {
        cout << "Your cart is empty.\n";
        return;
    }

    // Print the headers with Product ID, Name, Price, Quantity, and Total Price
    cout << "\n--- Your Cart ---\n";
    cout << left << setw(10) << "ProductID "
         << setw(20) << "ProductName"
         << setw(10) << "Price"
         << setw(10) << "Quantity"
         << setw(15) << "Total Price" << "\n";
    cout << string(65, '-') << "\n";

    float totalCartAmount = 0.0;
    for (int i = 0; i < cartCount; i++) {
        int productIndex = -1;

        // Find product details using the product ID
        for (int j = 0; j < productCount; j++) {
            if (productIDs[j] == cartIDs[i]) {
                productIndex = j;
                break;
            }
        }

        if (productIndex != -1) {
            float totalPrice = cartQuantities[i] * productPrices[productIndex];
            totalCartAmount += totalPrice;

            // Display product details
            cout << left << setw(10) << cartIDs[i]
                 << setw(20) << productNames[productIndex]
                 << setw(10) << productPrices[productIndex]
                 << setw(10) << cartQuantities[i]
                 << setw(15) << totalPrice << "\n";
        }
    }

    cout << string(65, '-') << "\n";
    cout << "Total Amount: " << totalCartAmount << "\n";
}

// Save products to a file
void saveProductsToFile() {
    ofstream productFile("products.txt");
    if (!productFile) {
        cout << "Error opening products file for saving.\n";
        return;
    }

    for (int i = 0; i < productCount; i++) {
        productFile << productIDs[i] << " "
                    << productNames[i] << " "
                    << productPrices[i] << " "
                    << productQuantities[i] << " "
                    << productCategories[i] << " "
                    << productDiscounts[i] << "\n";
    }

    productFile.close();
    cout << "Products saved to file successfully.\n";
}
// Load products from a file
void loadProductsFromFile() {
    ifstream productFile("products.txt");
    if (!productFile) {
        cout << "Products file not found. Starting with an empty inventory.\n";
        return;
    }

    productCount = 0; // Reset product count
    while (productFile >> productIDs[productCount]
                       >> productNames[productCount]
                       >> productPrices[productCount]
                       >> productQuantities[productCount]
                       >> productCategories[productCount]
                       >> productDiscounts[productCount]) {
        productCount++;
        if (productCount >= MAX_PRODUCTS) {
            cout << "Maximum product limit reached while loading from file.\n";
            break;
        }
    }

    productFile.close();
    cout << "Products loaded from file successfully.\n";
}


// Checkout process for cart
void checkout() {
    float totalAmount = 0.0;

    if (cartCount == 0) {
        cout << "Your cart is empty. Please add products to your cart before checking out.\n";
        return;
    }

    cout << "\n--- Checkout ---\n";

    // Open sales file in append mode to save new sales data
    ofstream salesFile("sales.txt", ios::app);
    if (!salesFile.is_open()) {
        cout << "Error opening sales file.\n";
        return;
    }

    // Get the current timestamp
    time_t currentTime = time(0);
    struct tm* timeInfo = localtime(&currentTime);

    for (int i = 0; i < cartCount; i++) {
        int productIndex = -1;

        // Find the product index in the product list based on product ID
        for (int j = 0; j < productCount; j++) {
            if (productIDs[j] == cartIDs[i]) {
                productIndex = j;
                break;
            }
        }

        // If the product is found in the product list
        if (productIndex != -1) {
            float productTotal = cartQuantities[i] * productPrices[productIndex];
            totalAmount += productTotal;

            // Print to console
            cout << "Product: " << productNames[productIndex]
                 << " | Quantity: " << cartQuantities[i]
                 << " | Price: " << productPrices[productIndex]
                 << " | Total: " << productTotal << "\n";

            // Save sales data to the sales file
            salesFile << productIDs[productIndex] << " "
                      << productNames[productIndex] << " "
                      << cartQuantities[i] << " "
                      << productPrices[productIndex] << " "
                      << productTotal << " "
                      << mktime(timeInfo) << "\n";

            // Deduct the sold quantity from the product inventory
            productQuantities[productIndex] -= cartQuantities[i];
        }
    }

    // Apply discount if any
    totalAmount = applyDiscount(totalAmount);

    cout << "Total amount: " << totalAmount << "\n";
    cout << "Checkout successful. Thank you for your purchase!\n";

    // Clear the cart after checkout
    cartCount = 0;

    // Save updated product inventory to the file
    saveProductsToFile();

    // Close the sales file
    salesFile.close();
}


// View sales data
void viewSales() {
    ifstream salesFile("sales.txt");

    // Check if the file opens successfully
    if (!salesFile.is_open()) {
        cout << "Error opening 'sales.txt'. Please ensure the file exists and is accessible.\n";
        return;
    }

    int productId, quantity;
    char productName[30];
    float price, total;
    time_t timestamp;

    // Print table headers
    cout << "\n--- Sales Records ---\n";
    cout << left << setw(10) << "Product ID"
         << setw(20) << "Product Name"
         << setw(10) << "Quantity"
         << setw(10) << "Price"
         << setw(10) << "Total"
         << "Date/Time\n";
    cout << string(90, '-') << "\n";

    // Read and display each record from the sales file
    bool hasRecords = false;
    while (salesFile >> productId >> productName >> quantity >> price >> total >> timestamp) {
        hasRecords = true;

        // Convert timestamp to a readable date and time
        char timeString[25];
        struct tm* timeInfo = localtime(&timestamp);
        if (timeInfo != nullptr) {
            strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
        } else {
            // Assign "Invalid Time" manually without strncpy
            const char* invalidTime = "Invalid Time";
            int i;
            for (i = 0; invalidTime[i] != '\0' && i < sizeof(timeString) - 1; i++) {
                timeString[i] = invalidTime[i];
            }
            timeString[i] = '\0';  // Null-terminate
        }

        // Display the record
        cout << left << setw(10) << productId
             << setw(20) << productName
             << setw(10) << quantity
             << setw(10) << price
             << setw(10) << total
             << timeString << "\n";
    }

    // If no records were found
    if (!hasRecords) {
        cout << "No sales records found.\n";
    }

    // Close the file
    salesFile.close();
}

//write Feedback
void writeFeedback() {
    string customerName;
    cout << "Enter your name: ";
    cin.ignore();  // Ignore leftover newline
    getline(cin, customerName);

    ofstream feedbackFile("feedback.txt", ios::app);
    if (!feedbackFile) {
        cout << "Error opening feedback file.\n";
        return;
    }

    string feedback;
    cout << "Please write your feedback: ";
    getline(cin, feedback);

    feedbackFile << "Customer: " << customerName << " | Feedback: " << feedback << "\n";
    feedbackFile.close();

    cout << "Thank you for your feedback, " << customerName << "!\n";
}

//Requst Product
void requestProduct() {
    char productName[30];
    int quantityRequested;
    
    cout << "Enter product name: ";
    cin >> productName;
    cout << "Enter quantity: ";
    cin >> quantityRequested;

    bool found = false;
    for (int i = 0; i < productCount; i++) {
        if (stringsEqual(productNames[i], productName)) {
            found = true;
            if (quantityRequested > productQuantities[i]) {
                cout << "Only " << productQuantities[i] << " units are available. Adjusting request.\n";
                quantityRequested = productQuantities[i];
            }

            // Log the request
            ofstream requestFile("product_requests.txt", ios::app);
            if (requestFile) {
                requestFile << "Product: " << productNames[i] 
                            << " | Requested Quantity: " << quantityRequested 
                            << " | Status: In Inventory\n";
                requestFile.close();
            }
            cout << "Request recorded for " << quantityRequested 
                 << " units of " << productNames[i] << ".\n";
            break;
        }
    }

    if (!found) {
        // Log the request for an unavailable product
        ofstream requestFile("product_requests.txt", ios::app);
        if (requestFile) {
            requestFile << "Product: " << productName 
                        << " | Requested Quantity: " << quantityRequested 
                        << " | Status: Not in Inventory\n";
            requestFile.close();
        }
        cout << "Product not found in inventory. Request recorded for " 
             << quantityRequested << " units of " << productName << ".\n";
    }
}


// Add a product to the wishlist
void addToWishlist() {
	int productId, quantity;
	cout << "Enter product ID to add to wishlist: ";
	cin >> productId;
	cout << "Enter quantity: ";
	cin >> quantity;

	for (int i = 0; i < productCount; i++) {
		if (productIDs[i] == productId) {
			wishlistIDs[wishlistCount] = productId;
			wishlistQuantities[wishlistCount] = quantity;
			wishlistCount++;
			cout << "Product added to wishlist.\n";
			return;
		}
	}

	cout << "Product ID not found.\n";
}


// Remove a product from the wishlist
void removeFromWishlist() {
	int productId;
	cout << "Enter product ID to remove from wishlist: ";
	cin >> productId;

	bool found = false;
	for (int i = 0; i < wishlistCount; i++) {
		if (wishlistIDs[i] == productId) {
			// Shift all elements after the removed product
			for (int j = i; j < wishlistCount - 1; j++) {
				wishlistIDs[j] = wishlistIDs[j + 1];
				wishlistQuantities[j] = wishlistQuantities[j + 1];
			}
			wishlistCount--;
			found = true;
			cout << "Product removed from wishlist.\n";
			break;
		}
	}

	if (!found) {
		cout << "Product not found in wishlist.\n";
	}
}


// View all products in the wishlist
void viewWishlist() {
    if (wishlistCount == 0) {
        cout << "Your wishlist is empty.\n";
        return;
    }

    // Print the headers with Product ID, Name, Price, Quantity, and Category
    cout << "\n--- Your Wishlist ---\n";
    cout << left << setw(10) << "Product ID"  // Product ID header
         << setw(20) << "Product Name"
         << setw(15) << "Price"
         << setw(15) << "Quantity"
         << setw(20) << "Category" << "\n";  // Category header
    cout << string(80, '-') << "\n";  // Line separator

    // Loop through the wishlist and display product details
    for (int i = 0; i < wishlistCount; i++) {
        int productIndex = -1;

        // Find product details using the product ID
        for (int j = 0; j < productCount; j++) {
            if (productIDs[j] == wishlistIDs[i]) {
                productIndex = j;
                break;
            }
        }

        if (productIndex != -1) {
            // Display product ID, name, price, quantity, and category
            cout << left << setw(10) << wishlistIDs[i]  // Display Product ID
                 << setw(20) << productNames[productIndex]
                 << setw(15) << productPrices[productIndex]
                 << setw(15) << wishlistQuantities[i]
                 << setw(20) << productCategories[productIndex] << "\n";  // Display Category
        }
    }
}


// Admin Menu
void adminMenu() {
	int choice;
	do {
		cout << "\nAdmin Menu:\n";
		cout << "1. View All Users\n";
		cout << "2. View Products\n";
		cout << "3. View Sales\n";
		cout << "4. Generate Promotion Code\n";
		cout << "5. Remove Promotion Code\n";
		cout << "6. View Promotion Code\n";
		cout << "7. Set Product Discount\n";
		cout << "8. Remove Product Discount\n";
		cout << "9. View Product Discount\n";
		cout << "10. View Failed Logins\n";
		cout << "11. View Product Request\n";
		cout << "12. View Feedback\n";
		cout << "13. Add Product\n";
		cout << "14. Delete Product\n";
		cout << "15. Restock Product\n";
		cout << "16. Logout\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			viewAllUsers();
			break;
		case 2:
			viewProducts();
			break;
		case 3:
			viewSales();
			break;
		case 4:
			generatePromoCode();
			break;
		case 5:
            removePromoCode();
            break;
        case 6:
			viewPromoCodes();
			break; 
		case 7:
			setProductDiscount();
			break;
		case 8:
			removeProductDiscount();
			break;
		case 9:
			viewProductDiscounts();
			break;
		case 10:
			viewFailedLogins();
			break;
		case 11:
			viewProductRequests();
			break;
		case 12:
			viewFeedback();
			break;
		case 13:
			addProduct();
			break;
		case 14:
			removeProduct();
			break;
		case 15:
			restockProduct();
			break;
		case 16:
			cout << "Logging out...\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 16);
}


// Customer Menu
void customerMenu() {
	int choice;
	do {
		cout << "\nCustomer Menu:\n";
		cout << "1. View Products\n";
		cout << "2. Add Product to Cart\n";
		cout << "3. Remove Product from Cart\n";
		cout << "4. View Cart\n";
		cout << "5. Checkout\n";
		cout << "6. View Wishlist\n";
		cout << "7. Add to Wishlist\n";
		cout << "8. Remove from Wishlist\n";
		cout << "9. Write Feedback\n";
		cout << "10. Request Product\n";
		cout << "11. Logout\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			viewProducts();
			break;
		case 2:
			addToCart();
			break;
		case 3:
			removeFromCart();
			break;
		case 4:
		    viewCart();
		    break;
		case 5:
			checkout();
			break;
		case 6:
			viewWishlist();
			break;
		case 7:
			addToWishlist();
			break;
		case 8:
			removeFromWishlist();
			break;
		case 9:
			writeFeedback();
			break;
		case 10:
			requestProduct();
			break;
		case 11:
			cout << "Logging out...\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 11);
}


// Employee Menu
void employeeMenu() {
	int choice;
	do {
		cout << "\nEmployee Menu:\n";
		cout << "1. View Products\n";
		cout << "2. Add Product\n";
		cout << "3. Remove Product\n";
		cout << "4. Restock Product\n";
		cout << "5. View Sales\n";
		cout << "6. View Feedback\n";
		cout << "7. Logout\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			viewProducts();
			break;
		case 2:
			addProduct();
			break;
		case 3:
			removeProduct();
			break;
		case 4:
			restockProduct();
			break;
		case 5:
			viewSales();
			break;
		case 6:
			viewFeedback();
			break;
		case 7:
			cout << "Logging out...\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 7);
}





