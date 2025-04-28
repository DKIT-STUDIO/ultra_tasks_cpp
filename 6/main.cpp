#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <map>

using namespace std;

class Patient {
private:
    string id;
    string name;
    string dob;
    string gender;
    string contact;
public:
    Patient(string id, string name, string dob, string gender, string contact)
        : id(id), name(name), dob(dob), gender(gender), contact(contact) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getContact() const { return contact; }
};

class Doctor {
private:
    string id;
    string name;
    string specialization;
    map<string, bool> schedule; // dateTime -> available
public:
    Doctor(string id, string name, string specialization)
        : id(id), name(name), specialization(specialization) {}
    
    void addAvailability(string dateTime) {
        schedule[dateTime] = true;
    }
    
    bool isAvailable(string dateTime) const {
        auto it = schedule.find(dateTime);
        return it != schedule.end() && it->second;
    }
    
    bool bookAppointment(string dateTime) {
        if (isAvailable(dateTime)) {
            schedule[dateTime] = false;
            return true;
        }
        return false;
    }
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getSpecialization() const { return specialization; }
};

class Appointment {
private:
    string id;
    Patient* patient;
    Doctor* doctor;
    string dateTime;
    string reason;
    string status;
public:
    Appointment(string id, Patient* patient, Doctor* doctor, string dateTime, string reason)
        : id(id), patient(patient), doctor(doctor), dateTime(dateTime), reason(reason), status("Scheduled") {}
    
    void cancel() {
        status = "Cancelled";
        doctor->addAvailability(dateTime);
    }
    
    void complete() {
        status = "Completed";
    }
    
    string getStatus() const { return status; }
    Doctor* getDoctor() const { return doctor; }
};

class MedicalRecord {
private:
    string id;
    Patient* patient;
    Doctor* doctor;
    string diagnosis;
    string treatment;
    time_t date;
public:
    MedicalRecord(string id, Patient* patient, Doctor* doctor, string diagnosis, string treatment)
        : id(id), patient(patient), doctor(doctor), diagnosis(diagnosis), treatment(treatment) {
        date = time(nullptr);
    }
    
    string getDiagnosis() const { return diagnosis; }
    string getTreatment() const { return treatment; }
};

class Prescription {
private:
    string id;
    Patient* patient;
    Doctor* doctor;
    vector<string> medications;
    string instructions;
    time_t date;
public:
    Prescription(string id, Patient* patient, Doctor* doctor, vector<string> meds, string instructions)
        : id(id), patient(patient), doctor(doctor), medications(meds), instructions(instructions) {
        date = time(nullptr);
    }
    
    void addMedication(string med) {
        medications.push_back(med);
    }
    
    vector<string> getMedications() const { return medications; }
};

class Billing {
private:
    string id;
    Patient* patient;
    double amount;
    string description;
    bool isPaid;
public:
    Billing(string id, Patient* patient, double amount, string description)
        : id(id), patient(patient), amount(amount), description(description), isPaid(false) {}
    
    void pay() {
        isPaid = true;
    }
    
    double getAmount() const { return amount; }
    bool paid() const { return isPaid; }
};

class MedicalSystem {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<MedicalRecord> records;
    vector<Prescription> prescriptions;
    vector<Billing> bills;
public:
    void addPatient(Patient patient) {
        patients.push_back(patient);
    }
    
    void addDoctor(Doctor doctor) {
        doctors.push_back(doctor);
    }
    
    string bookAppointment(Patient* patient, Doctor* doctor, string dateTime, string reason) {
        if (doctor->bookAppointment(dateTime)) {
            string id = "APP-" + to_string(appointments.size() + 1);
            appointments.emplace_back(id, patient, doctor, dateTime, reason);
            return id;
        }
        return "";
    }
    
    vector<Doctor*> findDoctorsBySpecialization(string specialization) {
        vector<Doctor*> result;
        for (auto& doc : doctors) {
            if (doc.getSpecialization() == specialization) {
                result.push_back(&doc);
            }
        }
        return result;
    }
    
    void createMedicalRecord(Patient* patient, Doctor* doctor, string diagnosis, string treatment) {
        string id = "MR-" + to_string(records.size() + 1);
        records.emplace_back(id, patient, doctor, diagnosis, treatment);
    }
    
    void createPrescription(Patient* patient, Doctor* doctor, vector<string> meds, string instructions) {
        string id = "RX-" + to_string(prescriptions.size() + 1);
        prescriptions.emplace_back(id, patient, doctor, meds, instructions);
    }
    
    void createBill(Patient* patient, double amount, string description) {
        string id = "BILL-" + to_string(bills.size() + 1);
        bills.emplace_back(id, patient, amount, description);
    }
};

int main() {
    MedicalSystem system;
    
    // Добавляем пациентов
    Patient p1("P1001", "John Doe", "1980-05-15", "Male", "john@example.com");
    Patient p2("P1002", "Jane Smith", "1990-08-22", "Female", "jane@example.com");
    system.addPatient(p1);
    system.addPatient(p2);
    
    // Добавляем врачей
    Doctor d1("D2001", "Dr. Smith", "Cardiology");
    d1.addAvailability("2023-06-01 10:00");
    d1.addAvailability("2023-06-01 11:00");
    system.addDoctor(d1);
    
    Doctor d2("D2002", "Dr. Johnson", "Neurology");
    d2.addAvailability("2023-06-02 09:00");
    system.addDoctor(d2);
    
    // Записываем на прием
    string apptId = system.bookAppointment(&p1, &d1, "2023-06-01 10:00", "Heart checkup");
    if (!apptId.empty()) {
        cout << "Appointment booked: " << apptId << endl;
    }
    
    // Ищем врачей по специализации
    auto cardiologists = system.findDoctorsBySpecialization("Cardiology");
    cout << "Found " << cardiologists.size() << " cardiologists" << endl;
    
    return 0;
}