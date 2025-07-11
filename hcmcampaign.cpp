#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////





//3.6 POSITION 
Position::Position(int r, int c) {
    this->r = r;
    this->c = c;
}

Position::Position(const string& str_pos) {
    int row = 0, col = 0;
    char ch;
    stringstream ss(str_pos);
    ss >> ch;     // Read '('
    ss >> row;    // Read row number
    ss >> ch;     // Read ','
    ss >> col;    // Read column number
    ss >> ch;     // Read ')'
    r = row;
    c = col;
}

int Position::getRow() const {
    return r;
}

int Position::getCol() const {
    return c;
}

void Position::setRow(int r) {
    this->r = r;
}

void Position::setCol(int c) {
    this->c = c;
}

string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}



//CLASS UNIT: 
Unit::Unit(int quantity, int weight, Position pos) {
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
}
Unit::~Unit() {}

void Unit::setAttackScore (int a) {
    attackScore=a; 
}

Position Unit::getCurrentPosition() const {
    return pos;
}

string Unit::str() const {
    return "Unit[quantity=" + to_string(quantity) +
           ", weight=" + to_string(weight) +
           ", pos=" + pos.str() + "]";
}

// Getters and Setters
int Unit::getQuantity() const {
    return quantity;
}
int Unit::getWeight() const {
    return weight;  
}

void Unit::setQuantity(int quantity) {
    this->quantity = quantity;
}
void Unit::setWeight(int weight) {
    this->weight = weight;
}









//3.2 PHƯƠNG TIỆN CHIẾN ĐẤU 

Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType) : Unit(quantity, weight, pos) {
    this->vehicleType = vehicleType;
}
Vehicle::~Vehicle() {}

string Vehicle:: VehicleType_string(VehicleType vehicle) const {
    switch (vehicle){
        case TRUCK: return "TRUCK";
        case MORTAR: return "MORTAR";
        case ANTIAIRCRAFT: return "ANTIAIRCRAFT";
        case ARMOREDCAR: return "ARMOREDCAR";
        case APC: return "APC";
        case ARTILLERY: return "ARTILLERY";
        case TANK: return "TANK";
        default: return "UNKNOWN";
    }
}

int Vehicle::getAttackScore() {
    int typeValue = static_cast<int>(vehicleType);
    double calculatedScore = (typeValue * 304 + quantity * weight)/(30.0);
    int score = static_cast<int>(ceil(calculatedScore));
    attackScore=score;
    return score;
}

int Vehicle::returnAttackScore() { 
    return attackScore; 
}


string Vehicle::str() const {
    
    return "Vehicle[vehicleType=" + VehicleType_string(vehicleType) +
           ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) +
           ",position=" + pos.str() + "]";
}

VehicleType Vehicle::getVehicleType() const {
    return vehicleType;
}

void Vehicle::setVehicleType(VehicleType vehicleType) {
    this->vehicleType = vehicleType;
}




//3.3 LỰC LƯỢNG BỘ BINH 

string Infantry:: InfantryType_string(InfantryType infantry) const {
    switch (infantry) {
        case SNIPER: return "SNIPER";
        case ANTIAIRCRAFTSQUAD: return "ANTIAIRCRAFTSQUAD";
        case MORTARSQUAD: return "MORTARSQUAD";
        case ENGINEER: return "ENGINEER";
        case SPECIALFORCES: return "SPECIALFORCES";
        case REGULARINFANTRY: return "REGULARINFANTRY";
        default: return "UNKNOWN";
    }
}

Infantry::Infantry(int quantity, int weight, Position pos, InfantryType infantryType) : Unit(quantity, weight, pos) {
    this->infantryType = infantryType;
}

Infantry::~Infantry() {}

int Infantry::getAttackScore() {
    
    int typeValue = static_cast<int>(infantryType);
    double calculatedScore = typeValue * 56 + quantity * weight;
    int score = static_cast<int>(ceil(calculatedScore));
    
    if (infantryType == SPECIALFORCES && sqrt(weight) == static_cast<int>(sqrt(weight))) {
        score += 75; 
    }

//Tính toán số cá nhân 
    int personal = computePersonalNumber(score, 1975);

    if (personal > 7) {
        // tăng 20%, làm tròn lên
        double tmp = quantity * 1.2;
        quantity = static_cast<int>(ceil(tmp));
    }
    else if (personal < 3) {
        // giảm 10%, làm tròn lên
        double tmp = quantity * 0.9;
        quantity = static_cast<int>(ceil(tmp));
    }

// Tính lại score sau khi quantity thay đổi
calculatedScore = typeValue * 56 + quantity * weight;
score = static_cast<int>(ceil(calculatedScore));
if (infantryType == SPECIALFORCES && sqrt(weight) == static_cast<int>(sqrt(weight))) {
    score += 75; 
}

    attackScore=score; 
    return score;
}

int Infantry::returnAttackScore() { 
    return attackScore; 
}


void Infantry::setInfantryType(InfantryType infantryType) {
    this->infantryType = infantryType;
}

InfantryType Infantry::getInfantryType() const {
    return infantryType;
}

string Infantry::str() const {
    static const string infantryNames[] = {
        "SNIPER", "ANTIAIRCRAFTSQUAD", "MORTARSQUAD", "ENGINEER",
        "SPECIALFORCES", "REGULARINFANTRY"
    };
    
    return "Infantry[infantryType=" + InfantryType_string(infantryType) +
           ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) +
           ",position=" + pos.str() + "]";
}

//Hàm tính tổng các chữ số của 1 số nguyên dương
int Infantry::sumDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

//tinh toan so ca nhan
int Infantry::computePersonalNumber(int score, int year) {
    int total = sumDigits(score) + sumDigits(year);
    // Tiếp tục làm tròn về số một chữ số
    while (total >= 10) {
        total = sumDigits(total);
    }
    return total;
}




    

    



//3.4 QUÂN ĐỘI 
Army::Army (Unit** unitArray, int size, string name, BattleField *battleField) {
    this->name = name;
    this->battleField = battleField;

    int initLF = 0;
    int initEXP = 0;

    for (int i = 0; i < size; ++i) {
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unitArray[i])) {
            // Nếu là Vehicle, cộng vào LF
            vehicle->getAttackScore(); // Cập nhật attackScore
            initLF += vehicle->getAttackScore();
            } 
            // Nếu là Infantry, cộng vào EXP
            else if (Infantry* infantry = dynamic_cast<Infantry*>(unitArray[i])) {
                infantry->getAttackScore(); // Cập nhật attackScore (cái này mới thêm vào để debug)
                initEXP += infantry->getAttackScore();
            }
    }

    LF = limit(initLF, 0, 1000);
    EXP = limit(initEXP, 0, 500);

    setLF(LF);
    setEXP(EXP);


    // Xác định sức chứa của UnitList
    int S = LF + EXP;
    bool isSpecial = false;
    for (int base : {3, 5, 7}) {
        int temp = S;
        bool special = true;
        while (temp > 0) {
            int digit = temp % base;
            if (digit > 1) {
                special = false;
                break;
            }
            temp /= base;
        }
        if (special) {
            isSpecial = true;
            break;
        }
    }
    int capacity;
    if (isSpecial) {
        capacity = 12;
    } else {
        capacity = 8;
    }

    // Khởi tạo unitList với sức chứa phù hợp
    this->unitList = new UnitList(capacity);


    for (int i = 0; i < size; ++i) {
            unitList->insert(unitArray[i]);
        }
/*
thêm vào để gỡ lỗi
*/
// --- áp dụng buff cho bất kỳ đơn vị Bộ binh nào đã được gộp ---
for (Unit* u : unitList->toVector()) {
    if (auto* inf = dynamic_cast<Infantry*>(u)) {
        inf->getAttackScore();  
        // tính lại attackScore VÀ áp dụng lại buff cho quantity
    }
}

}

int Army::getLF() const {
        return LF;
    }

void Army::setLF(int LF) {
        this->LF = limit(LF, 0, 1000);
    }

int Army::getEXP() const {
        return EXP;
    }

void Army::setEXP(int EXP) {
        this->EXP = limit(EXP, 0, 500);
    }

int Army::getInitLF() const {
        return initLF;
    }

int Army::getInitEXP() const {
        return initEXP;
    }

string Army::getName() const {
        return name;
    }

UnitList* Army::getUnitList() const {
    return unitList; 
}

BattleField* Army::getBattleField() const {
    return battleField; 
}

void Army::recalculateLFEXP () {
 LF = EXP = 0;
    for (Unit* u : unitList->toVector()) {
        int s = u->getAttackScore();
        if (dynamic_cast<Vehicle*>(u)) LF=LF+s;
        else EXP=EXP+s;
    }
    setLF(LF); 
    setEXP(EXP); 
}

//Hàm giới hạn giá trị trong khoảng [min, max]
int Army::limit(int value, int min, int max) const {
        if (value < min) {
            return min;
        } else if (value > max) {
            return max;
        }
        return value;
    }

void Army::removeUnits(const vector<Unit*>& toRemove) {
    for (Unit* u : toRemove) {
        unitList->remove(u);
    }
}

// Tìm tổ hợp các unit (Infantry hoặc Vehicle) sao cho tổng attackScore >= minScore và tổng nhỏ nhất có thể
vector<Unit*> Army::findCombination(UnitList* list, bool isInfantry, int minScore) {
    vector<Unit*> units;
    for (Unit* u : list->toVector()) {
        if (isInfantry && dynamic_cast<Infantry*>(u)) units.push_back(u);
        if (!isInfantry && dynamic_cast<Vehicle*>(u)) units.push_back(u);
    }
    int n = units.size();
    int bestSum = INT_MAX;
    vector<Unit*> bestCombo;

    // Duyệt tất cả các tập con (2^n)
    for (int mask = 1; mask < (1 << n); ++mask) {
        int sum = 0;
        vector<Unit*> combo;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                sum += units[i]->getAttackScore();
                combo.push_back(units[i]);
            }
        }
        if (sum >= minScore && sum < bestSum) {
            bestSum = sum;
            bestCombo = combo;
        }
    }
    return bestCombo;
}




//3.4.1 QUÂN GIẢI PHÓNG
LiberationArmy::LiberationArmy(Unit** unitArray, int size, string name, BattleField* battleField) : Army(unitArray, size, name, battleField) {}

void LiberationArmy::fight(Army * enemy , bool defense) {
    
    int oppLF = enemy->getLF();
    int oppEXP = enemy->getEXP();

if (defense==false) {
    double lfFactor = 1.5;
    double expFactor = 1.5;
    int ourLF = static_cast<int>(ceil(getLF() * lfFactor));
    int ourEXP = static_cast<int>(ceil(getEXP() * expFactor));

        auto A = findCombination(unitList, true, oppEXP);
        auto B = findCombination(unitList, false, oppLF);
        bool hasA = !A.empty();
        bool hasB = !B.empty();
        
        if (hasA && hasB) {
            removeUnits(A); 
            removeUnits(B); 
            //bắt giữ, trưng dụng 
            for (Unit* u : enemy->getUnitList()->reverseUnitList()) {
                unitList->insert(u);
            }
    // tịch thu xong, chỉ xoá các node khỏi enemy list, KHÔNG delete unit
    // (remove(u) chỉ delete Node chứ không delete cur->unit)
    for (Unit* u : enemy->getUnitList()->toVector()) {
        enemy->getUnitList()->remove(u);
    } 
            recalculateLFEXP();
        }

// Trường hợp chỉ có 1 tổ hợp thoả mãn
else if (hasA || hasB) {
    if (hasA && ourLF > oppLF) {
        removeUnits(A);

        // Xoá tất cả Vehicle còn lại (tổ hợp không thoả mãn)
        vector<Unit*> allVehicles;
        for (Unit* u : unitList->toVector()) {
            if (dynamic_cast<Vehicle*>(u)) allVehicles.push_back(u);
        }
        removeUnits(allVehicles);

        // Tịch thu các đơn vị của đối phương
        for (Unit* u : enemy->getUnitList()->reverseUnitList()) {
            unitList->insert(u);
        }
        enemy->getUnitList()->clear();
        recalculateLFEXP();
    }
    else if (hasB && ourEXP > oppEXP) {
        removeUnits(B); 

        // Xoá tất cả Infantry còn lại (tổ hợp không thoả mãn)
        vector<Unit*> allInfantry;
        for (Unit* u : unitList->toVector()) {
            if (dynamic_cast<Infantry*>(u)) allInfantry.push_back(u);
        }
        removeUnits(allInfantry);

        // Tịch thu các đơn vị của đối phương
        for (Unit* u : enemy->getUnitList()->reverseUnitList()) {
            unitList->insert(u);
        }
        enemy->getUnitList()->clear();
        recalculateLFEXP();
    }
}
    else {
    //khong danh: hao giam 10%

    for (Unit* u : unitList->toVector()) {
        u->setWeight(static_cast<int>(ceil((u->getWeight()) * 0.9)));
    }
    recalculateLFEXP();
    }
}

   
    else {
    double lfFactor = 1.3;
    double expFactor = 1.3;
    int ourLF = static_cast<int>(ceil(getLF() * lfFactor));
    int ourEXP = static_cast<int>(ceil(getEXP() * expFactor));

    if (ourLF >= oppLF && ourEXP >= oppEXP) {
        //trường hợp chiến thắng 
        recalculateLFEXP();
    } else if (ourLF >= oppLF || ourEXP >= oppEXP) {
        for (Unit* u : unitList->toVector()) {
            int q = u->getQuantity();
            u->setQuantity(static_cast<int>(ceil(q * 0.9)));
        }
        recalculateLFEXP();
    } else {
        for (Unit* u : unitList->toVector()) {
            u->setQuantity(fibNearest(u->getQuantity()));
        }
        recalculateLFEXP();
}
}
}

string LiberationArmy::str() const {
    stringstream ss;
    ss << "LiberationArmy[" 
       << "LF=" << LF
       << ",EXP=" << EXP
       << "," << unitList->str()
       << ",battleField=";
    if (battleField) ss << battleField->str();
    ss << "]";
    return ss.str();
}


/*
CÁC HÀM HỖ TRỢ LIBERATION ARMY
*/

int LiberationArmy::fibNearest(int x) const {
    if (x <= 1) return 1;
    int a = 1, b = 1;
    while (b < x) { int t = a + b; a = b; b = t; }
    return (b - x < x - a) ? b : a;
}


//3.4.2 QUÂN ĐỘI CHÍNH QUYỀN SÀI GÒN 
ARVN::ARVN (Unit ** unitArray , int size , string name , BattleField * battleField): Army (unitArray, size, name, battleField) {}

void ARVN::fight(Army* enemy, bool defense) {
    if (defense==false) //dù tấn công nhưng vẫn thua
    {
        vector<Unit*> units = unitList->toVector();
        for (Unit* unit : units) {
            int loseq=static_cast<int>(ceil((unit->getQuantity())*0.8)); 
            if (loseq<1) {
                unitList->remove(unit); 
            }
            else {
                unit->setQuantity(loseq);
            }
        }
        recalculateLFEXP(); 
    }
    
    
    //trường hợp ARVN phòng thủ nhưng luôn thua
    else {
    
    LiberationArmy* liber = dynamic_cast<LiberationArmy*>(enemy);
    if (!liber) {
        // Nếu không phải LiberationArmy, không giao tranh
        recalculateLFEXP();
        return;
    }

    // 1) Tìm tổ hợp A (infantry) và B (vehicle) của attacker
    int defenderEXP = getEXP();
    int defenderLF  = getLF();
    auto comboA = liber->findCombination(liber->getUnitList(), true,  defenderEXP);
    auto comboB = liber->findCombination(liber->getUnitList(), false, defenderLF);
    bool hasA = !comboA.empty();
    bool hasB = !comboB.empty();

    // 2) Xác định attackerWins theo đề (3.4.1)
    bool attackerWins = false;
    if (hasA && hasB) {
        attackerWins = true;
    } else if (hasA && !hasB) {
        if (enemy->getLF() > defenderLF) attackerWins = true;
    } else if (!hasA && hasB) {
        if (enemy->getEXP() > defenderEXP) attackerWins = true;
    }

    if (!attackerWins) {
        // Không đủ điều kiện → không giao tranh, giữ nguyên
        recalculateLFEXP();
        return;
    }

    UnitList* myList  = unitList;
    UnitList* oppList = liber->getUnitList();
    int cap     = oppList->getCapacity();
    int curSize = oppList->getCountVehicle() + oppList->getCountInfantry();

    if (liber) {
        liber->removeUnits(comboA);
        liber->removeUnits(comboB);
        liber->recalculateLFEXP();
    }

    // 3b) Trưng dụng các unit của ARVN từ cuối lên đầu, không vượt capacity
    vector<Unit*> seized; 
    for (Unit* u : myList->reverseUnitList()) {
        if (curSize >= cap) break;
        if (oppList->insert(u)) {
            ++curSize;
            seized.push_back(u);
        }
    }

    // 3c) Với các unit còn lại (không nằm trong 'seized'), giảm 20% trọng số
    for (Unit* u : myList->toVector()) {
    // tự tìm xem u đã bị trưng dụng chưa
    bool wasSeized = false;
    for (Unit* z : seized) {
        if (z == u) { 
            wasSeized = true; 
            break; 
        }
    }
    if (wasSeized==true) continue;

    int newW = static_cast<int>(ceil(u->getWeight() * 0.8));
    u->setWeight(newW);
}

    // 4) Cập nhật lại LF/EXP cho ARVN
    recalculateLFEXP();
}
}

string ARVN::str() const {
    stringstream ss;
    ss << "ARVN["
       << "LF=" << LF
       << ",EXP=" << EXP
       << "," << unitList->str()
       << ",battleField=";
    if (battleField) ss << battleField->str();
    ss << "]";
    return ss.str();
}



//3.5 DANH SÁCH CÁC ĐƠN VỊ QUÂN SỰ
UnitList::UnitList(int capacity) {
    this->capacity = capacity;      // Lưu sức chứa tối đa
    this->head = nullptr;           // Danh sách rỗng, không có phần tử đầu
    this->tail = nullptr;           // Không có phần tử cuối
    this->countVehicle = 0;         // Số phương tiện ban đầu là 0
    this->countInfantry = 0;        // Số bộ binh ban đầu là 0
}

int UnitList:: getCapacity() const {
    return capacity; 
}

int UnitList::getLF() const {
    int LF = 0;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(cur->unit)) {
            LF += vehicle->getAttackScore();
        }
    }
    return LF;
}

int UnitList::getEXP() const {
    int EXP = 0;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (Infantry* infantry = dynamic_cast<Infantry*>(cur->unit)) {
            EXP += infantry->getAttackScore();
        }
    }
    return EXP;
}

int UnitList::getCountVehicle() const {
    return countVehicle; 
}

void UnitList::setCountVechicle(int countVehicle) {
    this->countVehicle=countVehicle; 
}

int UnitList::getCountInfantry() const {
    return countInfantry; 
}

void UnitList::setCountInfantry(int countInfantry) {
    this->countInfantry=countInfantry;
}

UnitList::~UnitList() {
    // Node *cur = head;
    // while (cur) {
    //     Node *next = cur->next;
    //     // delete cur->unit; Giải phóng bộ nhớ cho đơn vị
    //     delete cur; // Giải phóng bộ nhớ cho nút
    //     cur = next;
    // }

    // clear(); (chỉnh lỗi )
}

//return true if insert successfully
bool UnitList:: insert(Unit *unit) {
    if (countVehicle + countInfantry >= capacity) return false;
    if (!unit) return false;

        // 1 Search for existing unit
        Node* cur = head;
        while (cur) {
            if (isSameType(cur->unit, unit)) {
                // update quantity
                int newQty = cur->unit->getQuantity() + unit->getQuantity();
                cur->unit->setQuantity(newQty);
                // delete unit;               // discard the passed‐in pointer
                return true;
            }
            cur = cur->next;
        }

        // 2 Not found → wrap in a new node
        Node* node = new Node(unit);

        // 3 Decide where to insert
        if (dynamic_cast<Vehicle*>(unit)) {
            countVehicle++; 
            // append at tail
            if (!head) {
                head = node;
            } else {
                Node* tail = head;
                while (tail->next) tail = tail->next;
                tail->next = node;
            }
        } else {
            countInfantry++;
            node->next = head;
            head = node;
        }
        return true;
} 

// return true if it exists
bool UnitList::isContain(VehicleType vehicleType) {
    Node* cur = head;
    while (cur) {
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(cur->unit)) {
            if (vehicle->getVehicleType() == vehicleType) {
                return true;
            }
        }
        cur = cur->next;
    }
    return false;
}

// return true if it exists
bool UnitList::isContain(InfantryType infantryType) {
    Node* cur = head;
    while (cur) {
        if (Infantry* infantry = dynamic_cast<Infantry*>(cur->unit)) {
            if (infantry->getInfantryType() == infantryType) {
                return true;
            }
        }
        cur = cur->next;
    }
    return false;
}

string UnitList:: str () const {
    int countVehicles = 0, countInfantries = 0;
    string unitListstr;

    Node* cur = head;
    while (cur) {
        if (dynamic_cast<Vehicle*>(cur->unit))      ++countVehicles;
        else if (dynamic_cast<Infantry*>(cur->unit)) ++countInfantries;
        unitListstr += cur->unit->str();
        if (cur->next) unitListstr += ",";
        cur = cur->next;
    }

    string result = 
        "unitList=UnitList[count_vehicle=" + to_string(countVehicles) +
        ";count_infantry=" + to_string(countInfantries);
    if (!unitListstr.empty()) {
        // chỉ chèn dấu chấm phẩy trước khi có unit
        result += ";" + unitListstr;
    }
    result += "]";
    return result;
}


vector<Unit*> UnitList::toVector() const {
    vector <Unit*> units; 
    Node* cur=head; 
    while (cur) {
        units.push_back(cur->unit); 
        cur=cur->next; 
    }
    return units; 
}

vector<Unit*> UnitList::reverseUnitList() const {
    vector<Unit*> units;
    Node* cur = head;

    // Lưu tất cả các đơn vị vào vector
    while (cur) {
        units.push_back(cur->unit);
        cur = cur->next;
    }

    // Tự đảo ngược vector
    int l = 0, r = units.size() - 1;
    while (l < r) {
        Unit* temp = units[l];
        units[l] = units[r];
        units[r] = temp;
        ++l;
        --r;
    }

    return units;
}

bool UnitList::isSameType(Unit* u1, Unit* u2) const {
    auto* v1 = dynamic_cast<Vehicle*>(u1);
    auto* v2 = dynamic_cast<Vehicle*>(u2);
    if (v1 && v2) return v1->getVehicleType() == v2->getVehicleType();

    auto* i1 = dynamic_cast<Infantry*>(u1);
    auto* i2 = dynamic_cast<Infantry*>(u2);
    if (i1 && i2) return i1->getInfantryType() == i2->getInfantryType();

    return false;
}

/*
xóa một đơn vị cụ thể (unit) khỏi danh sách liên kết UnitList. 
Nó không xóa toàn bộ danh sách, mà chỉ loại bỏ một phần tử nếu tìm thấy nó.
*/
void UnitList::remove(Unit* unit) {
    Node* prev=nullptr; 
    Node* cur=head; 
    while (cur!=nullptr) {
        if (cur->unit==unit) {
        /*
        Nếu cur là node đầu tiên (tức prev == nullptr):
        Khi đó, bạn không thể dùng prev->next vì prev chưa trỏ đến gì cả.

        Bạn phải cập nhật head để trỏ sang node kế tiếp → head = cur->next.
        */
            if (prev!=nullptr) {
                prev->next = cur->next;
            }
            else head=cur->next; 
            if (dynamic_cast<Vehicle*>(unit)) countVehicle--;
            else countInfantry--;
            delete cur;
            return;
        }
        prev=cur; 
        cur=cur->next; 
    }
}


void UnitList::clear()
{
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;
        // only delete the node, not the Unit*
        delete cur;
        cur = next;
    }
    head = nullptr;
    countVehicle = 0;
    countInfantry = 0;
}





//3.7 CÁC YẾU TỐ ĐỊA HÌNH CỦA TRẬN ĐỊA
TerrainElement::TerrainElement() {}

TerrainElement::TerrainElement(const Position &pos) {
    this->pos = pos;
}

Position TerrainElement::getPosition() const {
    return pos; 
}

TerrainElement::~TerrainElement() {
    // Destructor   
}

double TerrainElement::distance(const Position&a, const Position&b) const {
    return sqrt(pow(a.getRow() -b.getRow(), 2) + pow(a.getCol() - b.getCol(), 2));
}

//3.7.1 ĐƯỜNG
Road::Road (const Position& pos) : TerrainElement(pos) {}

void Road::getEffect(Army*army) {
    //không có gì xảy ra 
}

string Road::str() const {
    return "Road[pos="+pos.str()+"]"; 
} 

//3.7.2 NÚI
Mountain::Mountain (const Position&pos) : TerrainElement(pos) {}

void Mountain::getEffect(Army*army) {
    vector <Unit*> units = army->getUnitList()->toVector();
    bool isLiberation = dynamic_cast<LiberationArmy*>(army) != nullptr;

for (auto* unit: units) {
if (isLiberation==true) {
        double d=distance(unit->getCurrentPosition(), pos); 
        if (d<=2) { 
            if (dynamic_cast<Infantry*>(unit)!=nullptr) {
                int score=unit->returnAttackScore(); 
               army->setEXP(army->getEXP() +((int)(score*0.3))); 
            }
            else {
                if (dynamic_cast<Vehicle*>(unit) !=nullptr) {
                    int score=unit->returnAttackScore();
                    army->setLF(army->getLF()-((int)(score*0.1))); 
                }
            }
    }
}
else {
    double d=distance(unit->getCurrentPosition(), pos); 
    if (d<=4) {
        if (dynamic_cast<Infantry*>(unit)!=nullptr) {
            int score=unit->returnAttackScore(); 
            army->setEXP(army->getEXP() +((int)(score*0.2)));  
        }
        else {
            if (dynamic_cast<Vehicle*>(unit)!=nullptr) {
                int score=unit->returnAttackScore();
                army->setLF(army->getLF()-((int)(score*0.05))); 
            } 
        }
    }
}
}
}

string Mountain::str () const {
    return "Mountain[pos="+pos.str()+"]"; 
}

//3.7.3 SÔNG SÂU 
River::River (const Position& pos) : TerrainElement (pos) {}

void River::getEffect(Army* army) {
    vector <Unit*> units=army->getUnitList()->toVector(); 
    int ds=0; 
    for (auto* unit: units) {
        if (dynamic_cast<Infantry*>(unit)) {
            double d=distance(unit->getCurrentPosition(), pos); 
            if (d<=2) {
                int score=unit->returnAttackScore(); 
                unit->setAttackScore((int)(score*0.9)); 
            }
        }
    }
} //cần xem xét thêm

string River::str() const {
    return "River[pos="+pos.str()+"]"; 
}

//3.7.4 KHU DÂN CƯ
Urban::Urban (const Position& pos) : TerrainElement(pos) {}

void Urban :: getEffect(Army* army) {
    vector <Unit*> units=army->getUnitList()->toVector(); 
    bool isLiberation=dynamic_cast<LiberationArmy*>(army)!=nullptr;
    for (auto*unit: units) {
        double d=distance(unit->getCurrentPosition(), pos); 
        
    if (isLiberation==true) {
        if (auto* infantry=dynamic_cast<Infantry*>(unit)) {
            InfantryType check0=infantry->getInfantryType();
            int score=infantry->returnAttackScore();
            if (check0 == SPECIALFORCES || check0 == REGULARINFANTRY) {
                int radius = 5;
                if (d<=radius) {
                    int D= max(1, static_cast<int>(ceil(d)));
                    int increase = static_cast<int>(ceil((2.0 * score) / D));
                    unit->setAttackScore((int)(unit->returnAttackScore()+increase)); 
                }
            }
        }
            else if (auto*vehicle=dynamic_cast<Vehicle*>(unit)) {
                VehicleType check1=vehicle->getVehicleType(); 
                if (check1==ARTILLERY) { 
                    if (d<=2) {
                        unit->setAttackScore((int)((unit->returnAttackScore())*0.5)); 
                    }
                }
            }
        }
        else {
            if (auto* infantry=dynamic_cast<Infantry*>(unit)) {
                InfantryType check2=infantry->getInfantryType();
                int score=infantry->returnAttackScore();
                if (check2==REGULARINFANTRY) {
                    if (d<3) {
                        int D= max(1, static_cast<int>(ceil(d)));
                        int increase = static_cast<int>(ceil((3.0 * score) / (2.0 * D)));
                        unit->setAttackScore((int)(unit->returnAttackScore()+increase));
                    }
                }
            }
        }
    }
    
}

string Urban::str() const {
    return "Urban[pos="+pos.str()+"]";
}

//3.7.5 CHIEN HAO
Fortification::Fortification (const Position& pos) : TerrainElement(pos) {}

void Fortification::getEffect(Army* army) {
    vector<Unit*> units=army->getUnitList()->toVector(); 
    bool isLiberation=dynamic_cast<LiberationArmy*>(army)!=nullptr;
    for (auto* unit: units) {
        double d=distance(unit->getCurrentPosition(), pos); 
        if (isLiberation==true) {
        if (d<=2) {
            unit->setAttackScore((int)((unit->returnAttackScore())*0.8)); 
        }
    }
    else {
        if (d<=2) {
            unit->setAttackScore((int)((unit->returnAttackScore())*1.2)); 
        }
    }
}
}

string Fortification::str() const {
    return "Fortification[pos="+pos.str()+"]"; 
}

//3.7.6 KHU VỰC PHI QUAN SU
SpecialZone::SpecialZone (const Position& pos) : TerrainElement(pos) {}

void SpecialZone::getEffect(Army* army) {
    vector<Unit*> units = army->getUnitList()->toVector();
    for (auto* unit: units) {
    double d=distance(unit->getCurrentPosition(), pos);
    if (d<=1) {
        unit->setAttackScore(0); 
    }
}

}

string SpecialZone::str() const {
    return "SpecialZone[pos="+pos.str()+"]"; 
}




//3.8 TRẬN ĐỊA
BattleField::BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
    vector<Position*> arrayRiver, vector<Position*> arrayFortification,
    vector<Position*> arrayUrban, vector<Position*> arraySpecialZone) {
        this->n_rows=n_rows; 
        this->n_cols=n_cols; 
        
        //mang 2 chieu terrain
        terrain = new TerrainElement **[n_rows]; 
        for (int i=0; i<n_rows; i++) {
            terrain[i]= new TerrainElement*[n_cols]; 
            for (int j=0; j<n_cols; ++j) {
                //cho ban dau la Road
                terrain[i][j]=new Road(Position(i, j)); 
            }
        }

        for (auto pos: arrayForest) {
            terrain[pos->getRow()][pos->getCol()]=new Mountain(*pos); 
        }
        for (auto pos: arrayRiver) {
            terrain[pos->getRow()][pos->getCol()]=new River(*pos); 
        }
        for (auto pos: arrayFortification) {
            terrain[pos->getRow()][pos->getCol()]=new Fortification(*pos); 
        }
        for (auto pos: arrayUrban) {
            terrain[pos->getRow()][pos->getCol()]=new Urban(*pos); 
        }
        for (auto pos: arraySpecialZone) {
            terrain[pos->getRow()][pos->getCol()]=new SpecialZone(*pos); 
        }
}

BattleField::~BattleField() {
    for (int i=0; i<n_rows; i++) {
        for (int j=0; j<n_cols; j++) {
            delete terrain[i][j]; 
        }
        delete[] terrain[i];  
    }
    delete [] terrain; 
}

string BattleField::str() const {
    stringstream ss; 
    ss<<"BattleField[n_rows="<<n_rows<<", n_cols="<<n_cols<<"]";
    return ss.str();  
}

TerrainElement* BattleField::getTerrainAt(const Position& pos) const {
    return terrain[pos.getRow()][pos.getCol()];
}


    

//3.9 THIẾT LẬP 
Configuration:: Configuration(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Khong the mo file");
    }
    string line;

    vector<pair<string, string>> configList;

// Đọc từng dòng và lưu vào vector
while(getline(file, line)) {
    size_t pos = line.find('=');
    if (pos != string::npos) {
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        configList.emplace_back(key, value);
    }
}

// Tìm giá trị theo khóa
for (const auto& entry : configList) {
    if (entry.first == "NUM_ROWS") {
        n_rows = stoi(entry.second);
    } else if (entry.first == "NUM_COLS") {
        n_cols = stoi(entry.second);
    } else if (entry.first == "ARRAY_FOREST") {
        arrayForest = parsePositions(entry.second);
    } else if (entry.first == "ARRAY_RIVER") {
        arrayRiver = parsePositions(entry.second);
    } else if (entry.first == "ARRAY_FORTIFICATION") {
        arrayFortification = parsePositions(entry.second);
    } else if (entry.first == "ARRAY_URBAN") {
        arrayUrban = parsePositions(entry.second);
    } else if (entry.first == "ARRAY_SPECIAL_ZONE") {
        arraySpecialZone = parsePositions(entry.second);
    } else if (entry.first == "UNIT_LIST") {
        parseUnitList(entry.second);
    } else if (entry.first == "EVENT_CODE") {
        eventCode = validateEventCode(stoi(entry.second));
    }
}
    file.close();
}

Configuration::~Configuration() {
    for (auto p : arrayForest) delete p;
    for (auto p : arrayRiver) delete p;
    for (auto p : arrayFortification) delete p;
    for (auto p : arrayUrban) delete p;
    for (auto p : arraySpecialZone) delete p;

    // for (auto u : liberationUnits) delete u;
    // for (auto u : ARVNUnits) delete u;
}

string Configuration::str() const {
auto joinPos = [](const vector<Position*>& v) {
        string s;
        for (size_t i = 0; i < v.size(); ++i) {
            s += v[i]->str();
            if (i + 1 < v.size()) s += ","; // nếu chưa phải phần tử cuối thì thêm dấu phẩy
        }
        return s;
    };

auto joinUnit = [](const vector<Unit*>& v) {
        string s;
        for (size_t i = 0; i < v.size(); ++i) {
            s += v[i]->str();
            if (i + 1 < v.size()) s += ",";
        }
        return s;
    };

    stringstream ss;
    ss << "[num_rows=" << n_rows
       << ",num_cols=" << n_cols
       << ",arrayForest=[" << joinPos(arrayForest) << "]"
       << ",arrayRiver=[" << joinPos(arrayRiver) << "]"
       << ",arrayFortification=[" << joinPos(arrayFortification) << "]"
       << ",arrayUrban=[" << joinPos(arrayUrban) << "]"
       << ",arraySpecialZone=[" << joinPos(arraySpecialZone) << "]"
       << ",liberationUnits=[" << joinUnit(liberationUnits) << "]"
       << ",ARVNUnits=[" << joinUnit(ARVNUnits) << "]"
       << ",eventCode=" << eventCode
       << "]";

    return ss.str();
}

int Configuration::getRows() const { 
    return n_rows; 
}

int Configuration::getCols() const { 
    return n_cols; 
}

int Configuration::getEventCode() const { 
    return eventCode; 
}

const vector<Unit*>& Configuration::getLiberationUnits() const {
     return liberationUnits; 
}

const vector<Unit*>& Configuration::getARVNUnits() const { 
    return ARVNUnits; 
}

const vector<Position*>&Configuration:: getArrayForest() const {
     return arrayForest; 
}

const vector<Position*>& Configuration::getArrayRiver() const {
    return arrayRiver; 
}

const vector<Position*>& Configuration::getArrayFortification() const {
    return arrayFortification; 
}

const vector<Position*>& Configuration::getArrayUrban() const {
     return arrayUrban; 
}

const vector<Position*>& Configuration::getArraySpecialZone() const { 
    return arraySpecialZone; 
}

vector<Position*> Configuration:: parsePositions(const string& input) {
    vector<Position*> positions;
    size_t start = 0, end = 0;

    while ((start = input.find('(', end)) != string::npos) {
        end = input.find(')', start);
        if (end == string::npos) break;

        string positionStr = input.substr(start + 1, end - start - 1);
        size_t comma = positionStr.find(',');
        if (comma != string::npos) {
            int row = stoi(positionStr.substr(0, comma));
            int col = stoi(positionStr.substr(comma + 1));
            positions.emplace_back(new Position(row, col)); //trước đây là row, col thôi 
        }
    }
    return positions;
}

void Configuration::parseUnitList(const string& s) {
    // 0) Tìm vị trí của '[' và ']' trong chuỗi đầu vào
    auto start = s.find('[');
    auto end   = s.rfind(']');
    if (start == string::npos || end == string::npos || end <= start)
        return;

    // 1) Lấy phần giữa hai dấu ngoặc vuông
    string inner = s.substr(start + 1, end - start - 1);

    // 2) Tách thành các token UNIT(...) dựa vào depth của ngoặc đơn
    vector<string> tokens;
    string cur;
    int depth = 0;
    for (char ch : inner) {
        if (ch == '(') {
            ++depth;
            cur.push_back(ch);
        } else if (ch == ')') {
            --depth;
            cur.push_back(ch);
        } else if (ch == ',' && depth == 0) {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
        } else {
            cur.push_back(ch);
        }
    }
    if (!cur.empty()) {
        tokens.push_back(cur);
    }

    // 3) Xử lý từng token
    for (auto& tok : tokens) {
        // 3.1) Tách TYPE và phần param trong ngoặc
        auto p = tok.find('(');
        if (p == string::npos) continue;
        string type  = tok.substr(0, p);
        string param = tok.substr(p + 1, tok.size() - p - 2); // bỏ '(' và ')'

        // 3.2) Xóa khoảng trắng và tab trong param
        string tmp;
        tmp.reserve(param.size());
        for (char ch : param) {
            if (ch != ' ' && ch != '\t') tmp.push_back(ch);
        }
        param = tmp;

        // 3.3) Chia param thành 4 phần: quantity, weight, "(r,c)", armyIdx
        vector<string> parts;
        string part;
        int d = 0;
        for (char ch : param) {
            if (ch == '(') {
                ++d;
                part.push_back(ch);
            } else if (ch == ')') {
                --d;
                part.push_back(ch);
            } else if (ch == ',' && d == 0) {
                parts.push_back(part);
                part.clear();
            } else {
                part.push_back(ch);
            }
        }
        if (!part.empty()) {
            parts.push_back(part);
        }
        if (parts.size() != 4) 
        continue;  // không đúng định dạng

        // 3.4) Chuyển phần số thành int bằng stoi
        int q    = stoi(parts[0]);
        int w    = stoi(parts[1]);
        int r, c;
        {
            auto& rc = parts[2];         // dạng "(r,c)"
            auto comma = rc.find(',');
            r = stoi(rc.substr(1, comma - 1));
            c = stoi(rc.substr(comma + 1, rc.size() - comma - 2)); //
        }
        int armyIdx = stoi(parts[3]);

        // 3.5) Khởi tạo Unit* tương ứng
        Position pos(r, c);
        Unit* u = nullptr;
        if (type == "TANK") {u = new Vehicle(q, w, pos, TANK);}
        else if (type == "TRUCK") {u = new Vehicle(q, w, pos, TRUCK);}
        else if (type == "MORTAR") {u = new Vehicle(q, w, pos, MORTAR);}
        else if (type == "ANTIAIRCRAFT") {u = new Vehicle(q, w, pos, ANTIAIRCRAFT);}
        else if (type == "ARMOREDCAR"){u = new Vehicle(q, w, pos, ARMOREDCAR);}
        else if (type == "ARTILLERY") {u = new Vehicle(q, w, pos, ARTILLERY);}
        else if (type == "APC")  {u = new Vehicle(q, w, pos, APC);}
        else if (type == "SNIPER") {u = new Infantry(q, w, pos, SNIPER);}
        else if (type == "ANTIAIRCRAFTSQUAD") {u = new Infantry(q, w, pos, ANTIAIRCRAFTSQUAD);}
        else if (type == "MORTARSQUAD") {u = new Infantry(q, w, pos, MORTARSQUAD);}
        else if (type == "ENGINEER") {u = new Infantry(q, w, pos, ENGINEER);}
        else if (type == "SPECIALFORCES") {u = new Infantry(q, w, pos, SPECIALFORCES);}
        else if (type == "REGULARINFANTRY"){u = new Infantry(q, w, pos, REGULARINFANTRY);}

        // 3.6) Đẩy con trỏ vào vector tương ứng
        if (u) {
            if (armyIdx == 0) liberationUnits.push_back(u);
            else if (armyIdx == 1)  ARVNUnits.push_back(u);
        }
    }
}





//3.10 CLASS HCMCAMPAIGN 
HCMCampaign::HCMCampaign(const string &config_file_path) { 
    // 1) Đọc file cấu hình
    config = new Configuration(config_file_path);

    // 2) Tạo Battlefield
    battleField = new BattleField(
        config->getRows(),
        config->getCols(),
        config->getArrayForest(),
        config->getArrayRiver(),
        config->getArrayFortification(),
        config->getArrayUrban(),
        config->getArraySpecialZone()
    );

    // 3) Lấy danh sách Unit* từ config và dựng mảng để truyền vào ctor
    const auto &libUnits = config->getLiberationUnits();
    const auto &arvnUnits = config->getARVNUnits();

    // Chuyển vector<Unit*> thành mảng Unit**
    int nL = static_cast<int>(libUnits.size());
    Unit **libArr = new Unit*[nL];
    for (int i = 0; i < nL; ++i) libArr[i] = libUnits[i];

    int nA = static_cast<int>(arvnUnits.size());
    Unit **arvnArr = new Unit*[nA];
    for (int i = 0; i < nA; ++i) arvnArr[i] = arvnUnits[i];

    // 4) Khởi tạo hai Army
    liberationArmy = new LiberationArmy (libArr, nL, config->str(), battleField);
    arvn = new ARVN (arvnArr, nA, config->str(), battleField);

    // 5) Giải phóng mảng tạm
    delete[] libArr;
    delete[] arvnArr;
}

// run: thực hiện trận chiến theo eventCode và loại bỏ unit yếu
void HCMCampaign::run() {
    // 1) Áp hiệu ứng địa hình lên cả hai bên
    for (auto pos : config->getArrayForest())  {   
    battleField->getTerrainAt(*pos)->getEffect(liberationArmy); 
    battleField->getTerrainAt(*pos)->getEffect(arvn);
    }
    for (auto pos : config->getArrayRiver()) {
    battleField->getTerrainAt(*pos)->getEffect(liberationArmy);
    battleField->getTerrainAt(*pos)->getEffect(arvn);
    }
    for (auto pos : config->getArrayFortification()) {
    battleField->getTerrainAt(*pos)->getEffect(liberationArmy);
    battleField->getTerrainAt(*pos)->getEffect(arvn);
    }
    for (auto pos : config->getArrayUrban()) {       
    battleField->getTerrainAt(*pos)->getEffect(liberationArmy);
    battleField->getTerrainAt(*pos)->getEffect(arvn);
    }
    for (auto pos : config->getArraySpecialZone()) { 
    battleField->getTerrainAt(*pos)->getEffect(liberationArmy); 
    battleField->getTerrainAt(*pos)->getEffect(arvn);
    }

    // 2) Giao tranh theo eventCode
    int code = config->getEventCode();
    if (code < 75) {
        // Liberation tấn công, ARVN phòng thủ
        liberationArmy->fight(arvn, false);
        arvn->fight(liberationArmy, true);
    } else {
        // ARVN chủ động tấn công, rồi Liberation phản công
        arvn->fight(liberationArmy, false);
        liberationArmy->fight(arvn, true);
    }

    // 3) Loại bỏ unit có attackScore <= 5
    auto prune = [](Army* a) {
        vector<Unit*> units = a->getUnitList()->toVector();
        for (Unit* u : units) {
            if (u->returnAttackScore() <= 5) {
                a->getUnitList()->remove(u);
            }
        }
        a->recalculateLFEXP();
    };
    prune(liberationArmy);
    prune(arvn);
}

// printResult: in ra chuỗi kết quả cuối cùng
string HCMCampaign::printResult() {
    // Định dạng: LIBERATIONARMY[LF=<LF>,EXP=<EXP>]-ARVN[LF=<LF>,EXP=<EXP>]
    string s = "LIBERATIONARMY[LF=" + to_string(liberationArmy->getLF())
             + ",EXP=" + to_string(liberationArmy->getEXP()) + "]"
             + "-ARVN[LF=" + to_string(arvn->getLF())
             + ",EXP=" + to_string(arvn->getEXP()) + "]";
    return s;
}



//CÁC HÀM TẠO THÊM
//mục 3.9: 
int Configuration:: validateEventCode(int inputCode) {
    if (inputCode > 99) {
        return inputCode % 100; // Lấy 2 chữ số cuối nếu lớn hơn 99
    } else if (inputCode < 0) {
        return 0; // Nếu nhỏ hơn 0, mã sự kiện là 00
    }
    return inputCode; // Trả về mã sự kiện nếu nằm trong khoảng [00,99]
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////