
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Doctor {
public:
    int doctorId;
    string name;
    string specialization;
    double consultationFee;

    Doctor(int id, string doctorName, string speciality,
           double fee) {
        doctorId = id;
        name = doctorName;
        specialization = speciality;
        consultationFee = fee;
    }

    void display() const {
        cout << left << setw(10) << doctorId
             << setw(22) << name
             << setw(22) << specialization
             << "Rs. " << fixed << setprecision(2)
             << consultationFee << endl;
    }
};

class Patient {
public:
    int patientId;
    string name;
    int age;
    string illness;
    int doctorId;
    string appointmentDate;
    bool appointmentActive;

    Patient(int id, string patientName, int patientAge,
            string condition, int docId, string date) {
        patientId = id;
        name = patientName;
        age = patientAge;
        illness = condition;
        doctorId = docId;
        appointmentDate = date;
        appointmentActive = true;
    }

    void display() const {
        cout << "\nPatient ID: " << patientId << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Medical Condition: " << illness << endl;
        cout << "Doctor ID: " << doctorId << endl;
        cout << "Appointment Date: " << appointmentDate << endl;
        cout << "Appointment Status: "
             << (appointmentActive ? "Active" : "Cancelled")
             << endl;
    }
};

class HospitalSystem {
private:
    vector<Doctor> doctors;
    vector<Patient> patients;
    int nextPatientId;

    int findDoctor(int id) const {
        for (int i = 0; i < (int)doctors.size(); i++) {
            if (doctors[i].doctorId == id)
                return i;
        }
        return -1;
    }

    int findPatient(int id) const {
        for (int i = 0; i < (int)patients.size(); i++) {
            if (patients[i].patientId == id)
                return i;
        }
        return -1;
    }

public:
    HospitalSystem() {
        nextPatientId = 5001;

        doctors.emplace_back(
            101, "Dr. Kumar", "General Medicine", 300);

        doctors.emplace_back(
            102, "Dr. Priya", "Cardiology", 600);

        doctors.emplace_back(
            103, "Dr. Ahmed", "Orthopaedics", 450);

        doctors.emplace_back(
            104, "Dr. Meera", "Dermatology", 400);
    }

    void displayDoctors() const {
        cout << "\n========== DOCTOR LIST ==========\n";
        cout << left << setw(10) << "Doctor ID"
             << setw(22) << "Doctor Name"
             << setw(22) << "Specialization"
             << "Fee\n";

        for (const Doctor& doctor : doctors)
            doctor.display();
    }

    void registerPatient() {
        string name, illness;
        int age;

        cout << "\nEnter patient name: ";
        getline(cin >> ws, name);

        cout << "Enter age: ";
        cin >> age;

        if (age <= 0 || age > 120) {
            cout << "Invalid age!\n";
            return;
        }

        cout << "Enter medical condition: ";
        getline(cin >> ws, illness);

        patients.emplace_back(
            nextPatientId++, name, age,
            illness, 0, "Not booked");

        cout << "Patient registered successfully!\n";
        cout << "Patient ID: "
             << patients.back().patientId << endl;
    }

    void bookAppointment() {
        int patientId, doctorId;
        string date;

        cout << "\nEnter patient ID: ";
        cin >> patientId;

        int patientIndex = findPatient(patientId);

        if (patientIndex == -1) {
            cout << "Patient not found! Register first.\n";
            return;
        }

        if (patients[patientIndex].appointmentActive &&
            patients[patientIndex].doctorId != 0) {
            cout << "This patient already has an active appointment.\n";
            return;
        }

        displayDoctors();

        cout << "\nEnter doctor ID: ";
        cin >> doctorId;

        int doctorIndex = findDoctor(doctorId);

        if (doctorIndex == -1) {
            cout << "Doctor not found!\n";
            return;
        }

        cout << "Enter appointment date (DD-MM-YYYY): ";
        cin >> date;

        patients[patientIndex].doctorId = doctorId;
        patients[patientIndex].appointmentDate = date;
        patients[patientIndex].appointmentActive = true;

        cout << "\nAppointment booked successfully!\n";
        cout << "Patient: "
             << patients[patientIndex].name << endl;
        cout << "Doctor: "
             << doctors[doctorIndex].name << endl;
        cout << "Consultation Fee: Rs. "
             << fixed << setprecision(2)
             << doctors[doctorIndex].consultationFee << endl;
    }

    void cancelAppointment() {
        int id;

        cout << "\nEnter patient ID: ";
        cin >> id;

        int index = findPatient(id);

        if (index == -1) {
            cout << "Patient not found!\n";
            return;
        }

        if (!patients[index].appointmentActive ||
            patients[index].doctorId == 0) {
            cout << "No active appointment found.\n";
            return;
        }

        patients[index].appointmentActive = false;
        patients[index].doctorId = 0;
        patients[index].appointmentDate = "Not booked";

        cout << "Appointment cancelled successfully!\n";
    }

    void searchPatient() const {
        int id;

        cout << "\nEnter patient ID: ";
        cin >> id;

        int index = findPatient(id);

        if (index == -1) {
            cout << "Patient not found!\n";
            return;
        }

        patients[index].display();

        if (patients[index].doctorId != 0) {
            int docIndex = findDoctor(
                patients[index].doctorId);

            if (docIndex != -1) {
                cout << "Doctor Name: "
                     << doctors[docIndex].name << endl;
                cout << "Specialization: "
                     << doctors[docIndex].specialization << endl;
            }
        }
    }

    void displayAllPatients() const {
        if (patients.empty()) {
            cout << "\nNo patient records available.\n";
            return;
        }

        cout << "\n========== PATIENT RECORDS ==========\n";

        for (const Patient& patient : patients)
            patient.display();
    }

    void displaySummary() const {
        int activeAppointments = 0;
        int cancelledAppointments = 0;

        for (const Patient& patient : patients) {
            if (patient.appointmentActive &&
                patient.doctorId != 0)
                activeAppointments++;
            else if (!patient.appointmentActive)
                cancelledAppointments++;
        }

        cout << "\n========== HOSPITAL SUMMARY ==========\n";
        cout << "Total Doctors: " << doctors.size() << endl;
        cout << "Registered Patients: " << patients.size() << endl;
        cout << "Active Appointments: "
             << activeAppointments << endl;
        cout << "Cancelled Appointments: "
             << cancelledAppointments << endl;
    }

    void run() {
        int choice;

        do {
            cout << "\n===== HOSPITAL MANAGEMENT SYSTEM =====\n";
            cout << "1. Display Doctors\n";
            cout << "2. Register Patient\n";
            cout << "3. Book Appointment\n";
            cout << "4. Cancel Appointment\n";
            cout << "5. Search Patient\n";
            cout << "6. Display All Patients\n";
            cout << "7. Hospital Summary\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input!\n";
                break;
            }

            switch (choice) {
                case 1: displayDoctors(); break;
                case 2: registerPatient(); break;
                case 3: bookAppointment(); break;
                case 4: cancelAppointment(); break;
                case 5: searchPatient(); break;
                case 6: displayAllPatients(); break;
                case 7: displaySummary(); break;
                case 0:
                    cout << "Thank you for using the system!\n";
                    break;
                default:
                    cout << "Invalid choice! Try again.\n";
            }

        } while (choice != 0);
    }
};

int main() {
    HospitalSystem hospital;
    hospital.run();
    return 0;
}
