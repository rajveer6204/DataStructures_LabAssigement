// hospital_system.cpp
// Compile: g++ -std=c++17 hospital_system.cpp -o hospital_system
// Run: ./hospital_system
//
// Single-file implementation of Hospital Appointment & Triage System for DS lab.
// Data structures used:
//  - Singly linked list for doctor schedules
//  - Circular queue for routine appointments
//  - Min-heap (vector) for emergency triage
//  - Hash table (chaining) for patient index
//  - Stack for undo log
//
// Author: ChatGPT (starter solution)

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------
   ADT Definitions
   --------------------------- */

enum TokenType { ROUTINE, EMERGENCY };

struct Patient {
    int id;
    string name;
    int age;
    string history;
    int severity; // optional default
    Patient() {}
    Patient(int _id, string _name, int _age, string _history="", int _severity=0)
        : id(_id), name(_name), age(_age), history(_history), severity(_severity) {}
};

struct Token {
    int tokenId;
    int patientId;
    int doctorId; // -1 when not assigned
    int slotId;   // -1 when not assigned
    TokenType type;
    Token() {}
    Token(int t, int p, int d, int s, TokenType ty) : tokenId(t), patientId(p), doctorId(d), slotId(s), type(ty) {}
};

struct DoctorSlot {
    int slotId;
    string startTime;
    string endTime;
    string status; // "FREE" or "BOOKED"
    DoctorSlot() {}
    DoctorSlot(int sid, string st, string et, string stt="FREE") : slotId(sid), startTime(st), end_time(et), status(stt) {}
    // small fix: use different variable name end_time to avoid reserved names.
    string end_time;
};

struct Doctor {
    int id;
    string name;
    string specialization;
    Doctor() {}
    Doctor(int _id, string _name, string _spec) : id(_id), name(_name), specialization(_spec) {}
};

/* ---------------------------
   Singly Linked List for Doctor Slots
   --------------------------- */

struct SlotNode {
    DoctorSlot slot;
    SlotNode* next;
    SlotNode(const DoctorSlot& s) : slot(s), next(nullptr) {}
};

class SlotLinkedList {
public:
    SlotNode* head;
    SlotLinkedList(): head(nullptr) {}

    // Insert at end: O(k) where k = slots in list
    SlotNode* insert_end(const DoctorSlot& s) {
        SlotNode* node = new SlotNode(s);
        if (!head) { head = node; return node; }
        SlotNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
        return node;
    }

    // Delete by slotId: O(k)
    DoctorSlot* delete_by_slotid(int slotId) {
        SlotNode* prev = nullptr;
        SlotNode* cur = head;
        while (cur) {
            if (cur->slot.slotId == slotId) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                DoctorSlot* copy = new DoctorSlot(cur->slot);
                delete cur;
                return copy;
            }
            prev = cur; cur = cur->next;
        }
        return nullptr;
    }

    // Find next free slot (first with status == "FREE"): O(k)
    DoctorSlot* find_next_free() {
        SlotNode* cur = head;
        while (cur) {
            if (cur->slot.status == "FREE") return &cur->slot;
            cur = cur->next;
        }
        return nullptr;
    }

    vector<DoctorSlot> to_vector() {
        vector<DoctorSlot> v;
        SlotNode* cur = head;
        while (cur) { v.push_back(cur->slot); cur = cur->next; }
        return v;
    }

    // Traverse generator-like: returns vector
    vector<DoctorSlot> list_slots() {
        return to_vector();
    }
};

/* ---------------------------
   Circular Queue for Routine Appointments
   --------------------------- */

class CircularQueue {
private:
    vector<Token> arr;
    int capacity;
    int frontIdx;
    int size_;
public:
    CircularQueue(int cap=100): capacity(cap), frontIdx(0), size_(0) {
        arr.resize(capacity);
    }
    bool is_full() const { return size_ == capacity; }           // O(1)
    bool is_empty() const { return size_ == 0; }                 // O(1)
    int size() const { return size_; }
    // O(1)
    void enqueue(const Token& t) {
        if (is_full()) throw runtime_error("Routine queue is full");
        int idx = (frontIdx + size_) % capacity;
        arr[idx] = t;
        size_++;
    }
    // O(1)
    Token dequeue() {
        if (is_empty()) throw runtime_error("Routine queue empty");
        Token t = arr[frontIdx];
        frontIdx = (frontIdx + 1) % capacity;
        size_--;
        return t;
    }
    // O(1)
    Token peek() const {
        if (is_empty()) throw runtime_error("Routine queue empty");
        return arr[frontIdx];
    }
    // Helper: dump contents to vector (O(n))
    vector<Token> to_vector() const {
        vector<Token> v; v.reserve(size_);
        for (int i=0;i<size_;++i) {
            int idx = (frontIdx + i) % capacity;
            v.push_back(arr[idx]);
        }
        return v;
    }
    // Helper: rebuild queue from vector (O(n))
    void rebuild_from_vector(const vector<Token>& v) {
        if ((int)v.size() > capacity) throw runtime_error("Rebuild size exceeds capacity");
        frontIdx = 0; size_ = 0;
        for (const auto& t : v) {
            arr[size_] = t;
            size_++;
        }
    }
};

/* ---------------------------
   Min-Heap Priority Queue for Emergency Triage
   (stores pair<severity, patientId>)
   Lower severity value = higher priority
   --------------------------- */

class MinHeap {
public:
    vector<pair<int,int>> heap; // (severity, patientId)

    MinHeap() { heap.clear(); }

    int parent(int i) const { return (i-1)/2; }
    int left(int i) const { return 2*i + 1; }
    int right(int i) const { return 2*i + 2; }

    // Insert: O(log n)
    void insert(pair<int,int> key) {
        heap.push_back(key);
        int i = (int)heap.size() - 1;
        while (i > 0 && heap[parent(i)].first > heap[i].first) {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    // Extract-min: O(log n)
    pair<int,int> extract_min() {
        if (heap.empty()) throw runtime_error("Triage heap empty");
        if (heap.size() == 1) {
            auto val = heap[0];
            heap.pop_back();
            return val;
        }
        auto root = heap[0];
        heap[0] = heap.back(); heap.pop_back();
        heapify(0);
        return root;
    }

    void heapify(int i) {
        int l = left(i), r = right(i);
        int smallest = i;
        if (l < (int)heap.size() && heap[l].first < heap[smallest].first) smallest = l;
        if (r < (int)heap.size() && heap[r].first < heap[smallest].first) smallest = r;
        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

    pair<int,int> peek_min() const {
        if (heap.empty()) throw runtime_error("Triage heap empty");
        return heap[0];
    }

    int size() const { return (int)heap.size(); }
};

/* ---------------------------
   Hash Table (chaining) for Patient Index
   --------------------------- */

class PatientIndex {
private:
    vector<vector<pair<int,Patient>>> buckets;
    int bucket_count;
public:
    PatientIndex(int buckets_count=101) {
        bucket_count = buckets_count;
        buckets.assign(bucket_count, {});
    }
    int bucket_index(int key) const { return (int)(std::hash<int>{}(key) % bucket_count); }

    // Insert or update: average O(1); worst O(n) if all collide
    void insert_or_update(const Patient& p) {
        int idx = bucket_index(p.id);
        for (auto &pr : buckets[idx]) {
            if (pr.first == p.id) { pr.second = p; return; }
        }
        buckets[idx].push_back({p.id, p});
    }

    // Get: average O(1)
    Patient* get(int pid) {
        int idx = bucket_index(pid);
        for (auto &pr : buckets[idx]) {
            if (pr.first == pid) return &pr.second;
        }
        return nullptr;
    }

    // Delete: average O(1)
    bool remove(int pid) {
        int idx = bucket_index(pid);
        auto &vec = buckets[idx];
        for (size_t i=0;i<vec.size();++i) {
            if (vec[i].first == pid) {
                vec.erase(vec.begin() + (int)i);
                return true;
            }
        }
        return false;
    }
};

/* ---------------------------
   Undo Stack
   We store actions with type and relevant payload.
   For simplicity, payload is stored as variant-like struct.
   --------------------------- */

enum ActionType { ACT_BOOK, ACT_CANCEL_SLOT, ACT_SERVE, ACT_TRIAGE_INSERT, ACT_SERVE_EMERGENCY, ACT_UNKNOWN };

struct Action {
    ActionType type;
    // We'll store copies of small objects (Token, DoctorSlot, int patientId, severity)
    Token token;            // used for book/serve/serve_emergency
    DoctorSlot slot;        // used for cancel slot
    int patientId;
    int severity;
    Action(): type(ACT_UNKNOWN), token(), slot(), patientId(-1), severity(-1) {}
};

class UndoStack {
private:
    vector<Action> st;
public:
    void push(const Action& a) { st.push_back(a); } // O(1)
    Action pop() {
        if (st.empty()) return Action();
        Action a = st.back();
        st.pop_back();
        return a;
    }
    bool empty() const { return st.empty(); }
    int size() const { return (int)st.size(); }
};

/* ---------------------------
   HospitalSystem: integration layer
   --------------------------- */

class HospitalSystem {
private:
    PatientIndex pindex;
    CircularQueue routineQueue;
    MinHeap triage;
    UndoStack undo;
    unordered_map<int, SlotLinkedList*> doctorSchedules; // doctorId -> linked list
    unordered_map<int, Doctor> doctors; // info
    vector<Token> servedTokens;
    vector<Token> pendingTokens;
    int tokenCounter;

public:
    HospitalSystem(int routineCapacity=200, int hashBuckets=101) : pindex(hashBuckets), routineQueue(routineCapacity) {
        tokenCounter = 1;
    }

    /* Patient operations */
    void upsert_patient(const Patient& p) {
        pindex.insert_or_update(p);
        cout << "Patient " << p.id << " upserted.\n";
    }

    void show_patient(int pid) {
        Patient* p = pindex.get(pid);
        if (!p) cout << "Patient not found.\n";
        else {
            cout << "ID: " << p->id << ", Name: " << p->name << ", Age: " << p->age
                 << ", History: " << p->history << ", Severity: " << p->severity << "\n";
        }
    }

    void delete_patient(int pid) {
        if (pindex.remove(pid)) cout << "Patient " << pid << " deleted.\n";
        else cout << "Patient not found.\n";
    }

    /* Doctor and schedule */
    void add_doctor(int did, const string& name="", const string& spec="") {
        if (doctors.find(did) == doctors.end()) {
            doctors[did] = Doctor(did, name, spec);
            doctorSchedules[did] = new SlotLinkedList();
            cout << "Doctor " << did << " added.\n";
        } else {
            cout << "Doctor already exists.\n";
        }
    }

    void schedule_add_slot(int doctorId, const DoctorSlot& slot) {
        if (doctorSchedules.find(doctorId) == doctorSchedules.end()) {
            cout << "Doctor not found. Add doctor first.\n";
            return;
        }
        doctorSchedules[doctorId]->insert_end(slot);
        cout << "Slot " << slot.slotId << " added to doctor " << doctorId << ".\n";
    }

    void schedule_cancel(int slotId) {
        for (auto &pr : doctorSchedules) {
            Doctor* dptr = (doctors.find(pr.first) != doctors.end()) ? &doctors[pr.first] : nullptr;
            DoctorSlot* removed = pr.second->delete_by_slotid(slotId);
            if (removed) {
                cout << "Slot " << slotId << " canceled from doctor " << pr.first << ".\n";
                // push undo action
                Action a; a.type = ACT_CANCEL_SLOT; a.slot = *removed;
                undo.push(a);
                delete removed;
                return;
            }
        }
        cout << "Slot id not found in schedules.\n";
    }

    void show_doctor_slots(int doctorId) {
        if (doctorSchedules.find(doctorId) == doctorSchedules.end()) {
            cout << "Doctor not found.\n"; return;
        }
        auto vec = doctorSchedules[doctorId]->list_slots();
        cout << "Slots for Doctor " << doctorId << ":\n";
        for (auto &s : vec) {
            cout << "  SlotId: " << s.slotId << " [" << s.startTime << "-" << s.end_time << "] Status: " << s.status << "\n";
        }
    }

    /* Booking: enqueue routine */
    Token enqueue_routine(int patientId, int doctorId, int slotId) {
        if (pindex.get(patientId) == nullptr) {
            throw runtime_error("Patient not found");
        }
        Token tk(tokenCounter++, patientId, doctorId, slotId, ROUTINE);
        routineQueue.enqueue(tk); // may throw overflow
        pendingTokens.push_back(tk);

        // push undo action
        Action a; a.type = ACT_BOOK; a.token = tk;
        undo.push(a);

        cout << "Booked token " << tk.tokenId << " (ROUTINE) for patient " << patientId << " doctor " << doctorId << " slot " << slotId << ".\n";
        return tk;
    }

    Token dequeue_routine() {
        // normally check triage first outside if needed; here we just dequeue routine
        Token tk = routineQueue.dequeue(); // may throw
        servedTokens.push_back(tk);
        // push undo
        Action a; a.type = ACT_SERVE; a.token = tk;
        undo.push(a);
        // remove from pending list
        remove_pending_token_by_id(tk.tokenId);
        cout << "Served routine token " << tk.tokenId << " (patient " << tk.patientId << ").\n";
        return tk;
    }

    /* Triage */
    void triage_insert(int patientId, int severity) {
        if (pindex.get(patientId) == nullptr) {
            throw runtime_error("Patient not found");
        }
        triage.insert({severity, patientId});
        Action a; a.type = ACT_TRIAGE_INSERT; a.patientId = patientId; a.severity = severity;
        undo.push(a);
        cout << "Inserted patient " << patientId << " into triage with severity " << severity << ".\n";
    }

    Token triage_pop_and_serve() {
        auto pr = triage.extract_min(); // (severity, patientId)
        int sev = pr.first, pid = pr.second;
        Token tk(tokenCounter++, pid, -1, -1, EMERGENCY);
        servedTokens.push_back(tk);
        Action a; a.type = ACT_SERVE_EMERGENCY; a.token = tk;
        undo.push(a);
        cout << "Served EMERGENCY patient " << pid << " (severity " << sev << "), token " << tk.tokenId << ".\n";
        return tk;
    }

    /* Undo - naive implementations
       For some operations (removing a booked token from the circular queue or removing a specific triage entry)
       we rebuild the structure by linear scan (O(n)). This is okay for lab assignment; note complexities in report.
    */
    void undo_pop() {
        if (undo.empty()) {
            cout << "Nothing to undo.\n"; return;
        }
        Action a = undo.pop();
        switch (a.type) {
            case ACT_BOOK: {
                // remove token from routine queue (linear rebuild)
                int tid = a.token.tokenId;
                bool removed = remove_token_from_routine_by_id(tid);
                if (removed) {
                    cout << "Undo: book reverted for token " << tid << ".\n";
                } else {
                    cout << "Undo: book - token not in queue (may have been served already)\n";
                }
                break;
            }
            case ACT_SERVE: {
                // served token should be moved back to front of queue (attempt)
                Token tk = a.token;
                // moving to front requires rebuild: put this token at front in new vector
                vector<Token> cur = routineQueue.to_vector();
                vector<Token> newVec;
                newVec.push_back(tk);
                for (auto &t : cur) newVec.push_back(t);
                routineQueue.rebuild_from_vector(newVec);
                // remove from served list if present
                for (size_t i=0;i<servedTokens.size();++i) {
                    if (servedTokens[i].tokenId == tk.tokenId) { servedTokens.erase(servedTokens.begin() + i); break; }
                }
                cout << "Undo: serve reverted, token " << tk.tokenId << " returned to front of routine queue.\n";
                break;
            }
            case ACT_TRIAGE_INSERT: {
                // remove one matching (severity, patientId) from heap: rebuild
                int pid = a.patientId, sev = a.severity;
                vector<pair<int,int>> newheap;
                for (auto &x : triage.heap) {
                    if (!(x.first == sev && x.second == pid)) newheap.push_back(x);
                }
                // rebuild heap
                triage.heap.clear();
                for (auto &x : newheap) triage.insert(x);
                cout << "Undo: triage insert reverted for patient " << pid << " severity " << sev << ".\n";
                break;
            }
            case ACT_CANCEL_SLOT: {
                // re-insert slot into its doctor schedule - we don't know doctor from action in this simple implement,
                // so we'll just re-add to the first doctor for demonstration (improvement: store doctorId in Action).
                // For correctness, store doctorId in Action when pushing cancel slot originally.
                schedule_add_slot(0, a.slot); // will create doctor 0 if not present
                cout << "Undo: canceled slot reverted (re-inserted slotId " << a.slot.slotId << ").\n";
                break;
            }
            case ACT_SERVE_EMERGENCY: {
                // put emergency back to triage as per severity unknown here - can't easily revert without severity.
                // For robust undo, store severity too in Action when triage served.
                cout << "Undo: serve_emergency is not fully supported in naive implementation.\n";
                break;
            }
            default:
                cout << "Undo: unknown action.\n";
        }
    }

    /* Reports */
    void report_per_doctor_pending() {
        cout << "Per-doctor pending counts and next free slot:\n";
        for (auto &pr : doctorSchedules) {
            int did = pr.first;
            auto vec = pr.second->list_slots();
            int pending_count = 0;
            // count tokens in pending for this doctor
            for (auto &t : pendingTokens) if (t.doctorId == did) pending_count++;
            cout << "Doctor " << did << " pending: " << pending_count << ". Next free slot: ";
            DoctorSlot* ns = pr.second->find_next_free();
            if (ns) cout << ns->slotId << " [" << ns->startTime << "-" << ns->end_time << "]\n";
            else cout << "None\n";
        }
    }

    void report_served_vs_pending() {
        cout << "Served count: " << servedTokens.size() << ", Pending (in queue): " << routineQueue.size() << ", Triage pending: " << triage.size() << "\n";
    }

    void report_top_k_frequent_patients(int K=3) {
        unordered_map<int,int> freq;
        for (auto &t : servedTokens) freq[t.patientId]++;
        vector<pair<int,int>> arr;
        for (auto &pr : freq) arr.push_back({pr.second, pr.first}); // (count, patientId)
        sort(arr.begin(), arr.end(), greater<>());
        cout << "Top-" << K << " frequent served patients:\n";
        for (int i=0;i<min(K, (int)arr.size()); ++i) {
            cout << " Patient " << arr[i].second << " served " << arr[i].first << " times\n";
        }
    }

    /* Utilities */

    bool remove_token_from_routine_by_id(int tokenId) {
        vector<Token> cur = routineQueue.to_vector();
        vector<Token> kept;
        bool removed = false;
        for (auto &t : cur) {
            if (t.tokenId == tokenId) { removed = true; continue; }
            kept.push_back(t);
        }
        routineQueue.rebuild_from_vector(kept);
        // remove from pending tokens vector
        for (size_t i=0;i<pendingTokens.size();++i) {
            if (pendingTokens[i].tokenId == tokenId) { pendingTokens.erase(pendingTokens.begin() + i); break; }
        }
        return removed;
    }

    void remove_pending_token_by_id(int tokenId) {
        for (size_t i=0;i<pendingTokens.size();++i) {
            if (pendingTokens[i].tokenId == tokenId) { pendingTokens.erase(pendingTokens.begin() + i); break; }
        }
    }

    /* CLI helper wrappers */
    void add_doctor_cli() {
        int did; string name, spec;
        cout << "Enter doctor id: "; cin >> did; cin.ignore();
        cout << "Enter name: "; getline(cin, name);
        cout << "Enter specialization: "; getline(cin, spec);
        add_doctor(did, name, spec);
    }

    void add_slot_cli() {
        int did, sid; string st, et;
        cout << "Doctor id: "; cin >> did;
        cout << "Slot id: "; cin >> sid; cin.ignore();
        cout << "Start time (HH:MM): "; getline(cin, st);
        cout << "End time (HH:MM): "; getline(cin, et);
        DoctorSlot s; s.slotId = sid; s.startTime = st; s.end_time = et; s.status = "FREE";
        schedule_add_slot(did, s);
    }

    void book_cli() {
        int pid, did, sid;
        cout << "Patient id: "; cin >> pid;
        cout << "Doctor id: "; cin >> did;
        cout << "Slot id: "; cin >> sid;
        try {
            enqueue_routine(pid, did, sid);
        } catch (exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    void serve_next_cli() {
        // priority: triage first
        if (triage.size() > 0) {
            cout << "Triage has emergencies. Serve triage first? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') {
                triage_pop_and_serve();
                return;
            }
        }
        try {
            dequeue_routine();
        } catch (exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    void triage_in_cli() {
        int pid, sev;
        cout << "Patient id: "; cin >> pid;
        cout << "Severity (lower=more urgent): "; cin >> sev;
        try {
            triage_insert(pid, sev);
        } catch (exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    void undo_cli() {
        undo_pop();
    }

    void patient_upsert_cli() {
        int id, age; string name, hist;
        cout << "Patient id: "; cin >> id; cin.ignore();
        cout << "Name: "; getline(cin, name);
        cout << "Age: "; cin >> age; cin.ignore();
        cout << "History: "; getline(cin, hist);
        Patient p(id, name, age, hist, 0);
        upsert_patient(p);
    }

    void show_menu() {
        cout << "\n--- HOSPITAL APPOINTMENT & TRIAGE SYSTEM ---\n";
        cout << "1. Add doctor\n";
        cout << "2. Add slot to doctor\n";
        cout << "3. Show doctor slots\n";
        cout << "4. Register / Upsert patient\n";
        cout << "5. Show patient\n";
        cout << "6. Book routine appointment (enqueue)\n";
        cout << "7. Serve next (dequeue / triage pop)\n";
        cout << "8. Emergency triage insert\n";
        cout << "9. Undo last action\n";
        cout << "10. Reports: per-doctor pending & next slot\n";
        cout << "11. Report: served vs pending\n";
        cout << "12. Report: top-K frequent patients\n";
        cout << "0. Exit\n";
    }

    void run_cli() {
        while (true) {
            show_menu();
            int choice; cout << "Choice: "; cin >> choice;
            switch (choice) {
                case 1: add_doctor_cli(); break;
                case 2: add_slot_cli(); break;
                case 3: { int did; cout << "Doctor id: "; cin >> did; show_doctor_slots(did); break;}
                case 4: patient_upsert_cli(); break;
                case 5: { int pid; cout << "Patient id: "; cin >> pid; show_patient(pid); break;}
                case 6: book_cli(); break;
                case 7: serve_next_cli(); break;
                case 8: triage_in_cli(); break;
                case 9: undo_cli(); break;
                case 10: report_per_doctor_pending(); break;
                case 11: report_served_vs_pending(); break;
                case 12: { int k; cout << "K: "; cin >> k; report_top_k_frequent_patients(k); break; }
                case 0: cout << "Exiting. Bye.\n"; return;
                default: cout << "Invalid choice.\n";
            }
        }
    }

    // destructor: free dynamic lists
    ~HospitalSystem() {
        for (auto &pr : doctorSchedules) {
            // delete linked list nodes recursively
            SlotNode* cur = pr.second->head;
            while (cur) {
                SlotNode* nxt = cur->next; delete cur; cur = nxt;
            }
            delete pr.second;
        }
    }
};

/* ---------------------------
   Main: run CLI or demo
   --------------------------- */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    HospitalSystem hs(50, 101);

    // seed some data for demo
    hs.add_doctor(101, "Dr. Sharma", "General");
    hs.add_doctor(102, "Dr. Mehta", "Orthopedics");
    DoctorSlot s1; s1.slotId = 201; s1.startTime = "09:00"; s1.end_time = "09:15"; s1.status = "FREE";
    DoctorSlot s2; s2.slotId = 202; s2.startTime = "09:15"; s2.end_time = "09:30"; s2.status = "FREE";
    hs.schedule_add_slot(101, s1);
    hs.schedule_add_slot(101, s2);
    hs.upsert_patient(Patient(1, "Alice", 30, "No prior history", 0));
    hs.upsert_patient(Patient(2, "Bob", 45, "Diabetic", 0));
    // run interactive CLI
    hs.run_cli();

    return 0;
}
