Here's the proposed plan:

**Phase 1: UI Design (`RequestMoney.ui`)**

1.  **Mimic `SendMoneyPage.ui` Structure:**
    *   Add a `QLineEdit` for the target user's username (who will receive the request). Let's name it `lineEditTargetUser`.
    *   Add a `QDoubleSpinBox` for the requested amount. Let's name it `doubleSpinBoxAmount`.
    *   Add a `QLineEdit` or `QTextEdit` for an optional message to accompany the request (aligning with `PaymentRequest::message`). Let's name it `lineEditMessage`.
    *   Add a `QPushButton` to submit the payment request. Let's name it `pushButtonSendRequest`.
    *   Add a `QLabel` to display status messages (e.g., success, errors). Let's name it `labelStatus`.
    *   The existing `pushButtonBackToHome` will be retained.
2.  **Layout:**
    *   Arrange the new widgets logically within a `QVBoxLayout` or `QFormLayout` for a clean user interface, similar to `SendMoneyPage.ui`.
    *   Ensure the main widget `RequestMoney` has an appropriate window title like "Request Payment".

**Phase 2: Header File (`RequestMoney.h`)**

1.  **Include Dependencies:**
    *   `Database.h`.
    *   `User.h`.
    *   `PaymentRequest.h`.
    *   `<random>` and `<limits>`.
2.  **Member Variables:**
    *   `Ui::RequestMoney *ui;`.
    *   `Database* database;`.
3.  **Slots:**
    *   `void on_pushButtonSendRequest_clicked();`.
    *   `on_pushButtonBackToHome_clicked()`.
4.  **Signals:**
    *   `void navigateToHomeRequested();`.
    *   `void paymentRequestSent(PaymentRequest* request);`.

**Phase 3: Source File (`RequestMoney.cpp`)**

1.  **Fix Linter Errors:** Add `#include "ui_RequestMoney.h"`. Ensure `Ui` namespace forward declaration is correct in `RequestMoney.h`.
2.  **Constructor (`RequestMoney::RequestMoney`)**:
    *   Initialize `ui` and call `ui->setupUi(this)`.
    *   Get `Database` instance.
    *   Connect `ui->pushButtonSendRequest->clicked()` to `this, &RequestMoney::on_pushButtonSendRequest_clicked`.
3.  **Implement `on_pushButtonSendRequest_clicked()`**: Align with existing `User.h` methods.
    *   Use `requester->addPendingOutgoingRequest(*request);`
    *   Use `payer->addPendingIncomingRequest(*request);`
    *   Use `payer->addNotificationRequest(request);`
    *   Generate `int` ID for `PaymentRequest`.

**Phase 4: Core Class Modifications**

1.  **`Database.h/.cpp`**:
    *   **Storage:** Add `std::vector<PaymentRequest*> paymentRequests;` to store pointers to all `PaymentRequest` objects.
    *   **Method:** Implement `void addPaymentRequest(PaymentRequest* request);`. This method adds the pointer to the `paymentRequests` vector.
    *   **Consideration:** Getter methods like `PaymentRequest* getPaymentRequestById(int id);` and `const std::vector<PaymentRequest*>& getAllPaymentRequests() const;`.
    *   **Data Persistence:** Update saving/loading logic (e.g., `PaymentRequests.txt`). Serialize/deserialize `PaymentRequest` data including fields from `Event` (ID, amount, sender, recipient, status, message, date).
    *   **Cleanup:** In `Database::~Database()` or `Database::cleanUp()`, delete all `PaymentRequest` objects pointed to by `paymentRequests`.

2.  **`User.h/.cpp`**:
    *   **Storage (Existing):** `vector<PaymentRequest> pendingIncomingRequests;` and `vector<PaymentRequest> pendingOutgoingRequests;`.
    *   **Methods (Existing):** `addPendingIncomingRequest(PaymentRequest request);`, `addPendingOutgoingRequest(PaymentRequest request);`.
    *   **Notification System (Existing):** `void addNotificationRequest(PaymentRequest* request);`. The implementation of this method in `User.cpp` will need to create a `Notification` object from the `PaymentRequest` details and add it to the `notifications` queue.

3.  **`PaymentRequest.h/.cpp` (and `Event.h`)**:
    *   Ensure `Event::Event(...)` constructor initializes the `date` member (e.g., using `std::time(nullptr)`).
    *   `PaymentRequest` ID is `int`.
    *   `Event::setStatus` is available for later use.

**Phase 5: Build System & Integration**

1.  Ensure new `.cpp` files (if any from core changes) are in `CMakeLists.txt`.
2.  Run `uic` (via CMake/qmake) to generate `ui_RequestMoney.h` from `RequestMoney.ui`.

**Phase 6: Testing**

1.  Compile.
2.  Test UI and logic.
3.  Verify data storage and notifications.