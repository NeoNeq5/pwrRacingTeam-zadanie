#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

class Point;
vector<Point> readFile();
void findSubSet(vector<Point>);
double getAngle(Point, Point);
double getLength(Point, Point);
double getVectorProduct(Point, Point, Point);

int main() {
    vector<Point> points;
    points = readFile();
    findSubSet(points);

}
/**
 * @brief Klasa reprezentująca dwuwymiarowy punkt.
 *
 * Klasa przechowuje współrzędne x i y punktu.
 */
class Point {
    public:
    float x;
    float y;
};

/**
 * @brief Funkcja słóżąca do odczytu pliku i zapisaniu punktów w tablicy.
 *
 * @return 'points' jest to tablica punktów.
 */
vector<Point> readFile() {
    string fileName;
    vector<Point> points;
    cout<<"Podaj nazwe pliku"<<endl;
    cin>>fileName;
    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        int numPoints = 0;
        if (getline(file, line)) { // odczytanie pierwszej lini jako ilość punktów
            numPoints = stoi(line);
        }
        for (int i = 0; i < numPoints; i++) { //odczytywanie wszystkich punktów i przeniesienie ich do tablicy jako obiekty point
            if (getline(file, line)) {

                stringstream ss(line);
                string xStr, yStr;

                // Read x and y values from the line
                if (getline(ss, xStr, ' ') && getline(ss, yStr, ' ')) { // rozdzielenie lini na dwa stringi
                    float x = stof(xStr);
                    float y = stof(yStr);
                    points.push_back(Point{x, y});
                }
            }
        }

        file.close();
    }
    else {
        cout<<"Nie mozna otworzyc pliku"<<endl;
    }
    return points;
}

/**
 * @brief Funkcja znajduje podzbiór punktów wystarczający do zamknięcia reszty w punktów w swoim obrebie.
 *
 * Najpierw funkcja sortuje punkty względem kąta utworzonego z punktem bazowym i osią OX.
 * Później sprawdza 'skręt' następnego punktu względem dwóch sąsiadujących go punktów.
 * Jeżeli sket jest w prawą stronę następny punkt jest dodawany na stos,
 * jeżeli skręt jest w lewą stronę ostatni pubkt ze stosu jest usuwany a następny punkt jest dodawany na stos.
 *
 * @param points tablica punktów
 * @return  tablica z podzbiorem szukanych punktów
 */
void findSubSet(vector<Point> points) {
    // kod szukający punkt bazowy (najmniejszy y i mniejszy x)
    Point basePoint = points[0];
    for (int i = 0; i < points.size(); i++) {
        if(basePoint.y > points[i].y) {
            basePoint = points[i];
        }
        else if(basePoint.y == points[i].y) {
            if(basePoint.x > points[i].x) {
                basePoint = points[i];
            }
        }
    }
    //kod sprawdzający sortujący punkty przy pomocy getAngle
    for (int i = 1; i < points.size(); i++) {
        for (int j = i-1; j >= 0; j--) {
            if(getAngle(points[j], basePoint) > getAngle(points[j+1], basePoint)) { //jeżeli kąt points[j] jest większy niż kąt points[j+1] zamien punkty
                swap(points[j], points[j+1]);
            }
            else if (getAngle(points[j], basePoint) == getAngle(points[j+1], basePoint)) { //jeżeli kąt points[j] jest równy kątu points[j+1] sprawdz odległość punktów od punktu bazowego jeżeli pierwszy jest wieszy od drugiego zamien punkty
                if(getLength(points[j], basePoint) > getLength(points[j+1], basePoint)) {
                    swap(points[j], points[j+1]);
                }
            }
        }
    }
    vector<Point> sphere;

    if(points.size()<3){  // jeżeli jest za mało punktów przypisz tablice posortowanych wczesniej punktów
        sphere = points;
    }

    for (int i = 0; i < points.size(); i++) {   //pętla która dla każdego poprzedniego punktu sprawdza kierunek odchylenia i na tej podstawie tworzy nową tablicę
        if (sphere.size() > 1 && getVectorProduct(sphere[sphere.size()-2], sphere[sphere.size()-1], points[i]) <=0) {
            sphere.pop_back();
        }
        sphere.push_back(points[i]);
    }
    cout << "[";
    for (int i = 0; i < sphere.size(); i++) {
        cout << "(" << sphere[i].x << ", " << sphere[i].y << ")";
        if(i < sphere.size()-1) {
            cout << ", ";
        }
    }
    cout << "]";
}

/** @brief funkcja licząca iloczyn wektorowy pomiędzy trzema punktami
 *
 * @param a pierwszy z trzech punkt
 * @param b drugi z trzech punkt
 * @param c trzeci z trzech punkt
 * @return wartość iloczynu wektorowego
 */
double getVectorProduct(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/** @brief funkcja licząca kąt pomiędzy punktami
 *
 * @param point punkt do sprawdzenia kąta
 * @param basePoint punkt bazowy
 * @return kąt pomiędzy zadanym punktem, kątem bazowym oraz osią OX
 */
double getAngle(Point point, Point basePoint) {
    return atan2(point.y-basePoint.y, point.x-basePoint.x); //kod liczący arc tanges
}

/** @brief funkcja licząca odległosć dwóch punktó
 *
 * @param point punkt dla którego szukamy odległość od punktu bazowego
 * @param basePoint punkt bazowy
 * @return odległość od punktu bazowego
 */
double getLength(Point point, Point basePoint) {
    return sqrt(pow(point.x - basePoint.x, 2) + pow(point.y - basePoint.y, 2)); //wzór na odległość pomiędzy punktami
}
