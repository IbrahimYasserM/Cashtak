**Phase 1: UI Enhancements for Incoming Requests (`TransactionsHistoryPage`)**

1.  **Modify `TransactionsHistoryPage.ui` (if applicable, or do it programmatically):**
    *   The `tableWidgetTransactions` will need an additional column for "Actions" when displaying incoming requests.
2.  **Update `TransactionsHistoryPage.cpp - setupRequestTable()`:**
    *   Increase column count to 8 (ID, Date, Amount, From, To, Message, Status, Actions).
    *   Set the header label for the new column to "Actions".
    *   Adjust column widths as necessary.
3.  **Update `TransactionsHistoryPage.cpp - addRequestToTable(const PaymentRequest& request)`:**
    *   For the "Actions" column (index 7):
        *   Create a `QWidget` to act as a container for the buttons.
        *   Create a `QPushButton` named "Accept".
        *   Create a `QPushButton` named "Decline".
        *   Add both buttons to a `QHBoxLayout` within the container widget.
        *   Use `ui->tableWidgetTransactions->setCellWidget(row, 7, containerWidget);`.
    *   **Signal/Slot for Dynamic Buttons:**
        *   Connect `clicked()` signal of "Accept" button to `handleAcceptRequest(int requestId)`.
        *   Connect `clicked()` signal of "Decline" button to `handleDeclineRequest(int requestId)`.
        *   Use C++ lambdas capturing `request.getId()` for the connection.
4.  **Declare New Slots in `TransactionsHistoryPage.h`:**
    *   `private slots: void handleAcceptRequest(int requestId);`
    *   `private slots: void handleDeclineRequest(int requestId);`

**Phase 2: Core Logic for Modifying/Deleting Payment Requests (User-Side)**

1.  **`User.h` / `User.cpp`:**
    *   **New Methods for Removing Requests (ensure these correctly find and erase from the vectors):**
        *   `bool removePendingIncomingRequest(int requestId);` (iterates `pendingIncomingRequests`, erases if `request.getId() == requestId`, returns bool).
        *   `bool removePendingOutgoingRequest(int requestId);` (iterates `pendingOutgoingRequests`, erases if `request.getId() == requestId`, returns bool).
    *   **Note:** `PaymentRequest` objects are stored by value in `User`'s vectors. Removing them from the vector handles their lifecycle. Data persistence for these lists is via `pendingOutgoingRequests.txt` (loaded into both incoming/outgoing lists in `Database.cpp`).

**Phase 3: "Accept" Request Logic Implementation (`TransactionsHistoryPage.cpp`)**

1.  **Implement `TransactionsHistoryPage::handleAcceptRequest(int requestId)`:**
    *   Get current user (Payer) and the specific `PaymentRequest` object from their `pendingIncomingRequests` list (make a copy if needed before removal, or get all details first).
    *   Check Payer's balance against `request.getAmount()`. Show alert if insufficient.
    *   Get the original Requester (Payee) `User` object from `Database` using `request.getSender()`.
    *   **Perform Actions:**
        1.  Decrease Payer's balance (`payer->setBalance(...)`).
        2.  Increase Payee's balance (`payee->setBalance(...)`).
        3.  Create a new `CompletedTransaction` (generate new ID, use amount from request, Payer is sender of Tx, Payee is recipient of Tx, Status="Completed", Type="Request Fulfillment" or similar).
        4.  Add this new transaction to `database->addTransaction(newTransaction);`, `Payer->addTransaction(newTransaction);`, `Payee->addTransaction(newTransaction);`.
        5.  **Remove the `PaymentRequest` from user lists:**
            *   `Payer->removePendingIncomingRequest(requestId);`
            *   If Payee found: `Payee->removePendingOutgoingRequest(requestId);`
        6.  (Optional) Notify Payee of payment (e.g., `payee->addNotificationTransaction(newTransaction)`).
        7.  Update UI (show success message, refresh incoming requests view).

**Phase 4: "Decline" Request Logic Implementation (`TransactionsHistoryPage.cpp`)**

1.  **Implement `TransactionsHistoryPage::handleDeclineRequest(int requestId)`:**
    *   Get current user (Payer).
    *   Find the original sender (Payee) username from the request in `Payer->getPendingIncomingRequests()` before removing it.
    *   Get original Requester (Payee) `User` object from `Database` using the found sender username.
    *   **Perform Actions:**
        1.  **Remove the `PaymentRequest` from user lists:**
            *   `Payer->removePendingIncomingRequest(requestId);`
            *   If Payee found: `Payee->removePendingOutgoingRequest(requestId);`
        2.  (Optional) Notify Payee of decline (e.g., a new notification type or a generic message).
        3.  Update UI (show confirmation, refresh incoming requests view).

**Phase 5: Refinements and Helper methods**

1.  Robust Transaction ID generation for accepted requests.
2.  Detailed error handling and logging.
3.  Review `Database::getAccount()` for graceful failure.
4.  Ensure `removePendingIncomingRequest` and `removePendingOutgoingRequest` in `User.cpp` correctly find by ID and erase.

**Phase 6: Testing**

1.  **UI:** Actions column, button appearance and responsiveness.
2.  **Decline:** Verify request removal from Payer's incoming and Payee's outgoing lists. Verify persistence (request not in `pendingOutgoingRequests.txt` after save/reload).
3.  **Accept (Insufficient Balance):** Verify alert and no state changes.
4.  **Accept (Sufficient Balance):**
    *   Verify balance changes for Payer and Payee.
    *   Verify `CompletedTransaction` creation and addition to histories/Database.
    *   Verify `PaymentRequest` removal from Payer's incoming and Payee's outgoing lists.
    *   Verify data persistence for all changes (balances, new transaction, removed request from `pendingOutgoingRequests.txt`).
5.  **Edge Cases:** (e.g., other user deleted/inactive during the process).
