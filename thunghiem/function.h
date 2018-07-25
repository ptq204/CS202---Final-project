#include "library.h"
//for function with short code, we give it in dot h file for easy tracking
vector<float> splitRGB(string s);//helper function for reading SVG file
void splitTransform(Graphics& graphics, string s);
void splitPath(string s);
vector<PointF> splitPoint(string s);
//ATTRIBUTE CLASS
class Attribute
{
protected:
	float value;
	vector<float> arr;
	vector<PointF> Points;
	string transformList;
public:
	virtual string getName() = 0;
	virtual void setValue(string s) = 0;
	float getValue();
	vector<float> getArrValue();
	vector<PointF> getArrPoint();
	string getStringValue();
};
class stroke_color : public Attribute
{
public:
	stroke_color() {//white: 255,255,255
		for (int i = 0; i < 3; ++i) arr.push_back(255);
	}
	string getName() { return "stroke"; }
	void setValue(string s);
};

class fill_color : public Attribute
{
public:
	fill_color() {
		for (int i = 0; i < 3; ++i) arr.push_back(0);
	}
	string getName() { return "fill"; }
	void setValue(string s);
};

class transform : public Attribute
{
public:
	string getName() { return "transform"; }
	void setValue(string s);
};

class x1 : public Attribute
{
public:
	string getName() { return "x1"; }
	void setValue(string s) { value = stof(s); }
};

class y_1 : public Attribute
{
public:
	string getName() { return "y1"; }
	void setValue(string s) { value = stof(s); }
};

class y2 : public Attribute
{
public:
	string getName() { return "y2"; }
	void setValue(string s) { value = stof(s); }
};

class x2 : public Attribute
{
public:
	string getName() { return "x2"; }
	void setValue(string s) { value = stof(s); }
};

class cx : public Attribute
{
public:
	string getName() { return "cx"; }
	void setValue(string s) { value = stof(s); }
};

class cy : public Attribute
{
public:
	string getName() { return "cy"; }
	void setValue(string s) { value = stof(s); }
};

class rx : public Attribute
{
public:
	string getName() { return "rx"; }
	void setValue(string s) { value = stof(s); }
};

class ry : public Attribute
{
public:
	string getName() { return "ry"; }
	void setValue(string s) { value = stof(s); }
};

class r : public Attribute
{
public:
	string getName() { return "r"; }
	void setValue(string s) { value = stof(s); }
};

class x : public Attribute
{
public:
	string getName() { return "x"; }
	void setValue(string s) { value = stof(s); }
};

class y : public Attribute
{
public:
	string getName() { return "y"; }
	void setValue(string s) { value = stof(s); }
};

class stroke_width : public Attribute
{
public:
	stroke_width() {
		value = 0;
	}
	string getName() { return "stroke-width"; }
	void setValue(string s) { value = stof(s); }
};

class stroke_opacity : public Attribute
{
public:
	stroke_opacity() {
		value = 0;
	}
	string getName() { return "stroke-opacity"; }
	void setValue(string s) { value = stof(s); }
};

class width : public Attribute
{
public:
	string getName() { return "width"; }
	void setValue(string s) { value = stof(s); }
};

class height : public Attribute
{
public:
	string getName() { return "height"; }
	void setValue(string s) { value = stof(s); }
};

class fill_opacity : public Attribute
{
public:
	fill_opacity() {
		value = 1;
	}
	string getName() { return "fill-opacity"; }
	void setValue(string s) { value = stof(s); }
};
class d :public Attribute {
public:
	string getName() { return "d"; }
	void setValue(string s) { transformList = s; }
};
class points : public Attribute {
public:
	string getName() { return "points"; }
	void setValue(string s) {
		Points = splitPoint(s);
	}
};
class TextLine :public Attribute {
public:
	string getName() { return "textline"; }
	void setValue(string s) {
		transformList = s;
	}
};
class FontSize :public Attribute {
public:
	string getName() { return "font-size"; }
	void setValue(string s) {
		value = stof(s);
	}
};
enum attri { X1, Y1, X2, Y2, X, Y, CX, cY, RX, RY, R, WIDTH, HEIGHT, STROKE_WIDTH, STROKE_OPACITY, FILL_OPACITY, STROKE_COLOR, FILL_COLOR, TRANSFORM, D, PS, TEXTLINE, FONTSIZE };
//SHAPE
class Shape {
	static vector<Shape*> sampleObjects;
protected:
	vector<Attribute*> attributes;
	static void addSample(Shape* pFig);
public:
	~Shape() {
		sampleObjects.clear();
		attributes.clear();
	}
	Shape();
	virtual void setAttribute(string attr_name, string attr_value);
	virtual string getName() = 0;
	virtual Shape* Clone() = 0;
	static Shape* createObject(string clsName);
	Pen* getPen();
	Brush* getBrush();
	virtual VOID Draw(HDC hdc) = 0;
	virtual void Input(vector<Attribute*> old, xml_node<> *&proot);
};
//GROUP
class Group :public Shape {
	vector<Shape*> arr;
public:
	Group() {
		addSample(this);
	}
	~Group() {
		arr.clear();
	}
	Group(const Group& src) :Shape() {
		for (int i = 0; i < src.arr.size(); ++i) arr.push_back(src.arr[i]);
	}
	string getName() {
		return "g";
	}
	Shape* Clone() {
		return new Group(*this);
	}
	void Input(vector<Attribute*> old, xml_node<> *&proot);
	VOID Draw(HDC hdc) {
		for (int i = 0; i < arr.size(); ++i) {
			arr[i]->Draw(hdc);
		}
	}
};
//GROUP
//LINE
class Line :public Shape {
public:
	Line() {
		addSample(this);
	}
	Line(const Line& src) :Shape() {}
	string getName() {
		return "line";
	}
	Shape* Clone() {
		return new Line(*this);
	}
	VOID Draw(HDC hdc);
};
//LINE
//RECT
class HCN :public Shape {
public:
	HCN() {
		addSample(this);
	}
	HCN(const HCN& src) :Shape() {}
	string getName() {
		return "rect";
	}
	Shape* Clone() {
		return new HCN(*this);
	}
	VOID Draw(HDC hdc);
};
//RECT
//ELIP
class Elip :public Shape {
public:
	Elip() {
		addSample(this);
	}
	Elip(const Elip& src) :Shape() {}
	string getName() {
		return "ellipse";
	}
	Shape* Clone() {
		return new Elip(*this);
	}
	VOID Draw(HDC hdc);
};
//ELIP
//CIRCLE
class Circle : public Shape
{
public:
	Circle() {
		addSample(this);
	}
	Circle(const Circle& src) :Shape() { }
	string getName() {
		return "circle";
	}
	Shape* Clone() {
		return new Circle(*this);
	}
	VOID Draw(HDC hdc);
};
//CIRCLE
//POLYLINE
class PolyLine :public Shape {
public:
	PolyLine() {
		addSample(this);
	}
	PolyLine(const PolyLine& src) :Shape() {}
	string getName() {
		return "polyline";
	}
	Shape* Clone() {
		return new PolyLine(*this);
	}
	VOID Draw(HDC hdc);
};
//POLYLINE
//POLYGON
class DaGiac :public Shape {
public:
	DaGiac() {
		addSample(this);
	}
	DaGiac(const DaGiac& src) :Shape() {}
	string getName() {
		return "polygon";
	}
	Shape* Clone() {
		return new DaGiac(*this);
	}
	VOID Draw(HDC hdc);
};
//POLYGON
//PATH
class Path :public Shape {
public:
	Path() {
		addSample(this);
	}
	Path(const Path& src) :Shape() {}
	string getName() { return "path"; }
	Shape* Clone() {
		return new Path(*this);
	}
	VOID Draw(HDC hdc);
};
//POLYGON
//TEXT
class Text :public Shape {
public:
	Text() {
		addSample(this);
	}
	Text(const Text& src) :Shape() {}
	string getName() { return "text"; }
	Shape* Clone() {
		return new Text(*this);
	}
	VOID Draw(HDC hdc);
};
void inputList(Group &g);
void copyVector(vector<Attribute*>&old, vector<Attribute*>& attributes);