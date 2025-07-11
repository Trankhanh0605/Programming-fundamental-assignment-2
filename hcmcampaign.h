/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration

//3.6
class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};


class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType {
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};

enum InfantryType {
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

//3.1
class Unit {
protected:
    int quantity, weight;
    Position pos;

public: 
    int attackScore; //thêm biến attackscore
public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;

    //tạo hàm ảo mới attackscore
    virtual int returnAttackScore()=0;  
    
    void setAttackScore(int a); 

    Position getCurrentPosition() const;
    virtual string str() const = 0;
    int getQuantity() const;
    int getWeight() const;
    void setQuantity(int quantity);
    void setWeight(int weight);
};


//CLASS VEHICLE 
class Vehicle : public Unit
{
private:
    VehicleType vehicleType;
    
public:

    Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType);
    ~Vehicle();

    string VehicleType_string(VehicleType vehicle) const; 

    int getAttackScore() override;

    int returnAttackScore() override; 

    string str() const override;
    VehicleType getVehicleType() const;
    void setVehicleType(VehicleType vehicleType);

};

//CLASS INFANTRY
class Infantry : public Unit
{
private:
    InfantryType infantryType;
public:
    Infantry(int quantity, int weight, Position pos, InfantryType infantryType);
    ~Infantry();

    string InfantryType_string(InfantryType infantry) const; 

    int getAttackScore() override;

    int returnAttackScore() override;

    string str() const override;
    InfantryType getInfantryType() const;
    void setInfantryType(InfantryType infantryType);
    // hàm tính tổng các chữ số của 1 số nguyên dương
    int sumDigits(int n);
    //ham tinh toan so ca nhan
    int computePersonalNumber(int score, int year);
};

//3.4
class Army
{
public: 
int initLF; 
int initEXP; 

protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;

    void recalculateLFEXP();

    vector<Unit*>findCombination(UnitList* list, bool isInfantry, int minScore); 

    int getLF() const;
    void setLF(int LF);
    int getEXP() const; 
    void setEXP(int EXP);
    //thêm
    //Hàm giới hạn giá trị trong khoảng [min, max]
    int limit(int value, int min, int max) const;

    void removeUnits(const vector<Unit*>& toRemove);

    int getInitLF() const;
    int getInitEXP() const;
    string getName() const;
    UnitList* getUnitList() const;
    BattleField* getBattleField() const;


};

//3.4.1
class LiberationArmy: public Army {
    
    public:
    LiberationArmy(Unit** unitArray, int size, string name, BattleField* battleField);
    void fight ( Army * enemy , bool defense = false ) override;
    string str () const override;


    private: //ho tro 
    
    int fibNearest(int x) const;

    

};

//3.4.2
class ARVN: public Army {
public: 
    ARVN (Unit ** unitArray , int size , string name , BattleField * battleField); 
    void fight(Army * enemy , bool defense = false ) override;
    string str() const override; 
    
}; 

//khai bao nút
struct Node
    {
        Unit *unit;
        Node *next;
        Node (Unit* unit) {
            this->unit=unit; 
            this->next=nullptr; 
        }
    };
//3.5 
class UnitList
{
private:
    int capacity;
    // TODO
    int countVehicle; 
    int countInfantry; 
    Node *head;
    Node *tail;


public:
    UnitList(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;

    // TODO
    ~UnitList();

    int getCapacity() const; 

    int getLF() const; 
    int getEXP() const; 
    int getCountVehicle() const;
    void setCountVechicle(int countVehicle) ; 
    
    int getCountInfantry() const;
    void setCountInfantry(int countInfantry) ; 
    
    
    vector<Unit*> toVector () const; //chuyển dổi link list sang dạng vector

    vector<Unit*> reverseUnitList() const; //chuyển đổi danh sách đơn vị quân sự theo thứ tự ngược lại 

    bool isSameType(Unit* u1, Unit* u2) const; 

    void remove(Unit* unit); 

    void clear(); 

   
    

};




//3.7
class TerrainElement
{
protected:
Position pos; 

public:
    TerrainElement();
    TerrainElement(const Position &pos);
    virtual ~TerrainElement(); //thêm virtual

    virtual void getEffect(Army *army) = 0;
    Position getPosition() const;
    virtual string str() const = 0;
    //ham lay khoang cach giua 2 diem
    double distance (const Position &a, const Position&b) const;
};

class Road : public TerrainElement {
    public:
        Road(const Position& pos);
        void getEffect(Army* army) override;
        string str() const override;
    };
    
class Mountain : public TerrainElement {
    public:
        Mountain(const Position& pos);
        void getEffect(Army* army) override;
        string str() const override;
    };
    
class River : public TerrainElement {
    public:
        River(const Position& pos);
        void getEffect(Army* army) override;
        string str() const override;
    };
    
class Urban : public TerrainElement {
    public:
        Urban(const Position& pos);
        void getEffect(Army* army) override;
        string str() const override;
    };
    
class Fortification : public TerrainElement {
    public:
        Fortification(const Position& pos);
        void getEffect(Army* army) override;
        string str() const override;
    };
    
class SpecialZone : public TerrainElement {
    public:
        SpecialZone(const Position& pos);
        void getEffect(Army* army) override;
        string str() const override;
    };

//3.8
class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
    TerrainElement *** terrain;  
public:
    BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
                vector<Position*> arrayRiver, vector<Position*> arrayFortification,
                vector<Position*> arrayUrban, vector<Position*> arraySpecialZone);
    ~BattleField();
    string str() const; 
    TerrainElement* getTerrainAt(const Position& pos) const;

    
};


//3.9
class Configuration {
    private:
    int n_rows, n_cols;
    vector<Position*> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    vector<Unit*> liberationUnits;
    vector<Unit*> ARVNUnits;
    int eventCode;
    void parseUnitList(const string& s);

public:
    Configuration(const string& filepath);
    ~Configuration();
    string str() const;
    int getRows() const;
    int getCols() const;
    int getEventCode() const;
    const vector<Unit*>& getLiberationUnits() const;
    const vector<Unit*>& getARVNUnits() const;
    const vector<Position*>& getArrayForest() const;
    const vector<Position*>& getArrayRiver() const;
    const vector<Position*>& getArrayFortification() const;
    const vector<Position*>& getArrayUrban() const;
    const vector<Position*>& getArraySpecialZone() const;

    vector<Position*> parsePositions(const string& input); 

    int validateEventCode(int inputCode); 
};



class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvn;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};

#endif