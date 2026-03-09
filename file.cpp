#include <iostream>
#include <string>
using namespace std;
string menuName[6]  = {"Burger", "Pizza", "Cold Coffee", "Lemon Soda", "Vanilla Ice Cream", "Chocolate Ice Cream"};
float  menuPrice[6] = {80, 150, 60, 40, 50, 60};
int    orderId[50], orderTable[50], orderItem[50], orderQty[50];
float  orderAmount[50];
string orderStatus[50];
int    totalItems = 0, nextOrderId = 1;

void showMenu() {
    cout << "\n--- Menu ---" << endl;
    for (int i = 0; i < 6; i++)
        cout << i+1 << ". " << menuName[i] << "  -  Rs." << menuPrice[i] << endl;
    cout << "------------" << endl;
}

void takeOrder() {
    int table, itemNo, qty;
    char more = 'y';
    int id = nextOrderId++;

    cout << "\nTable number : "; cin >> table;
    showMenu();

    while (more == 'y' || more == 'Y') {
        cout << "Which item? (1-6) : "; cin >> itemNo;
        if (itemNo < 1 || itemNo > 6) { cout << "That item doesn't exist, try again." << endl; continue; }

        cout << "How many? : "; cin >> qty;
        if (qty <= 0) { cout << "Quantity can't be zero or less." << endl; continue; }

        orderId[totalItems]     = id;
        orderTable[totalItems]  = table;
        orderItem[totalItems]   = itemNo - 1;
        orderQty[totalItems]    = qty;
        orderAmount[totalItems] = menuPrice[itemNo-1] * qty;
        orderStatus[totalItems] = "Pending";
        totalItems++;

        cout << menuName[itemNo-1] << " x" << qty << " added." << endl;
        cout << "Anything else? (y/n) : "; cin >> more;
    }
    cout << "Order " << id << " placed for table " << table << ". Status : Pending" << endl;
}

void viewOrders() {
    if (totalItems == 0) { cout << "\nNo orders placed yet." << endl; return; }
    cout << "\n--- All Orders ---" << endl;
    int prevId = -1;
    for (int i = 0; i < totalItems; i++) {
        if (orderId[i] != prevId) {
            cout << "\nOrder " << orderId[i] << " | Table " << orderTable[i] << " | Status : " << orderStatus[i] << endl;
            prevId = orderId[i];
        }
        cout << "  [Line " << i+1 << "] " << menuName[orderItem[i]]
             << " x" << orderQty[i] << " = Rs." << orderAmount[i]
             << " (" << orderStatus[i] << ")" << endl;
    }
    cout << "------------------" << endl;
}

void kitchenDisplay() {
    cout << "\n--- Kitchen ---" << endl;
    bool found = false;
    int prevId = -1;
    for (int i = 0; i < totalItems; i++) {
        if (orderStatus[i] != "Pending" && orderStatus[i] != "Preparing") continue;
        if (orderId[i] != prevId) { cout << "\nOrder " << orderId[i] << " | Table " << orderTable[i] << endl; prevId = orderId[i]; }
        cout << "  -> " << menuName[orderItem[i]] << " x" << orderQty[i] << " (" << orderStatus[i] << ")" << endl;
        found = true;
    }
    if (!found) { cout << "Kitchen is clear, no pending orders right now." << endl; return; }

    int id, choice;
    cout << "\nEnter order ID to update (0 to go back) : "; cin >> id;
    if (id == 0) return;
    cout << "1. Mark as Preparing   2. Mark as Ready : "; cin >> choice;

    if (choice == 1 || choice == 2) {
        string s = (choice == 1) ? "Preparing" : "Ready";
        for (int i = 0; i < totalItems; i++)
            if (orderId[i] == id) orderStatus[i] = s;
        cout << "Order " << id << " updated to " << s << "." << endl;
    }
    else cout << "Invalid choice." << endl;
}

void cancelItem() {
    cout << "\n--- Cancel an Item ---" << endl;
    int prevId = -1;
    for (int i = 0; i < totalItems; i++) {
        if (orderStatus[i] == "Cancelled" || orderStatus[i] == "Billed") continue;
        if (orderId[i] != prevId) { cout << "\nOrder " << orderId[i] << " | Table " << orderTable[i] << endl; prevId = orderId[i]; }
        cout << "  [Line " << i+1 << "] " << menuName[orderItem[i]] << " x" << orderQty[i] << " = Rs." << orderAmount[i] << endl;
    }

    int lineNo;
    cout << "\nEnter line number to cancel (0 to go back) : "; cin >> lineNo;
    if (lineNo == 0) return;
    lineNo--;

    if (lineNo < 0 || lineNo >= totalItems) { cout << "That line number doesn't exist." << endl; return; }
    if (orderStatus[lineNo] == "Billed" || orderStatus[lineNo] == "Cancelled") { cout << "Can't cancel this item anymore." << endl; return; }

    orderStatus[lineNo] = "Cancelled";
    cout << menuName[orderItem[lineNo]] << " has been removed from the order." << endl;
}

void generateBill() {
    cout << "\n--- Bill ---" << endl;
    bool anyReady = false;
    int prevId = -1;
    for (int i = 0; i < totalItems; i++) {
        if (orderStatus[i] == "Ready" && orderId[i] != prevId) {
            cout << "Order " << orderId[i] << " | Table " << orderTable[i] << endl;
            prevId = orderId[i]; anyReady = true;
        }
    }
    if (!anyReady) { cout << "No orders are ready for billing yet." << endl; return; }

    int id;
    cout << "Enter order ID to bill : "; cin >> id;

    float subtotal = 0; bool found = false;
    for (int i = 0; i < totalItems; i++)
        if (orderId[i] == id && orderStatus[i] == "Ready") { subtotal += orderAmount[i]; found = true; }

    if (!found) { cout << "Order not found or not ready yet." << endl; return; }

    float tax = subtotal * 0.05;
    cout << "\n---- Bill for Order " << id << " ----" << endl;
    for (int i = 0; i < totalItems; i++)
        if (orderId[i] == id && orderStatus[i] == "Ready")
            cout << menuName[orderItem[i]] << " x" << orderQty[i] << " = Rs." << orderAmount[i] << endl;
    cout << "------------------------------" << endl;
    cout << "Subtotal : Rs." << subtotal << endl;
    cout << "GST (5%) : Rs." << subtotal * 0.05 << endl;
    cout << "Total    : Rs." << subtotal + tax << endl;
    cout << "------------------------------" << endl;
    cout << "Thanks for dining with us, see you again!" << endl;

    for (int i = 0; i < totalItems; i++)
        if (orderId[i] == id) orderStatus[i] = "Billed";
}

int main() {
    cout << "================================" << endl;
    cout << "  Restaurant Management System"  << endl;
    cout << "================================" << endl;

    int choice;
    do {
        cout << "\n1. Show Menu"     << endl;
        cout << "2. Take Order"      << endl;
        cout << "3. Kitchen Display" << endl;
        cout << "4. Generate Bill"   << endl;
        cout << "5. View All Orders" << endl;
        cout << "6. Cancel Item"     << endl;
        cout << "7. Exit"            << endl;
        cout << "\nWhat would you like to do? "; cin >> choice;

        if      (choice == 1) showMenu();
        else if (choice == 2) takeOrder();
        else if (choice == 3) kitchenDisplay();
        else if (choice == 4) generateBill();
        else if (choice == 5) viewOrders();
        else if (choice == 6) cancelItem();
        else if (choice == 7) cout << "Closing system. Goodbye!" << endl;
        else                  cout << "Please enter a number between 1 and 7." << endl;

    } while (choice != 7);
    return 0;
}