#include <iostream>
#include <cmath>

using namespace std;

 class Polynomial {
 protected:
 
 class Term {
 
 protected:
  int exponent;
  int coefficient;
  Term *next;
  Term(int exp, int coeff, Term *n):exponent(exp), coefficient(coeff), next(n){};
  friend class Polynomial;
 
 public:
  int getexp(){return exponent;};
  int getcoef(){return coefficient;};
  Term *getnext(){return next;};
 };

 public:
 
 Polynomial();
 Polynomial(const Polynomial &p);
 ~Polynomial();

 Polynomial& operator=(const Polynomial &p); 

 void addTerm(int expon, int coeff);
 double evaluate(double x);

 friend Polynomial operator+ (const Polynomial &p, const Polynomial &q);
 friend Polynomial operator* (const Polynomial &p, const Polynomial &q);

 friend ostream & operator << (ostream &out, const Polynomial &p);
 
 class iterator{
  public:
  iterator(const iterator &i):ptr(i.ptr){};
  
  int getItExp(){return ptr->exponent;};
  int getItCoef(){return ptr->coefficient;};
  
  iterator & operator++(){
    ptr = ptr->next;
    return *this;
  };

  bool operator==(const iterator &i) const { return ptr == i.ptr; }
  bool operator!=(const iterator &i) const { return ptr != i.ptr; } 
  
  private:
  Term *ptr;
  iterator(Term *p): ptr(p){};
  friend class Polynomial;

};

iterator begin() const{ return iterator(front); };
iterator end() const{ return iterator(nullptr); };

 private:
 Term *front;

 void copy(const Polynomial &p){
   for(Term *r = p.front; r != nullptr; r = r->next)
     addTerm(r->exponent, r->coefficient);
     }

 void purge(){
   Term *r = front;
   while(r != nullptr){
     Term *q = r;
     r = r->next;
     delete q;
   }
 };

 };

 Polynomial::Polynomial():front(nullptr){};

 Polynomial::Polynomial(const Polynomial &p):front(nullptr) {copy(p);};

 Polynomial::~Polynomial(){
   //purge();
   delete front;};

 Polynomial& Polynomial:: operator= (const Polynomial &p){
   purge();
   front = nullptr;
   copy(p);
   return *this;
 };

 void Polynomial:: addTerm(int expon, int coeff){
     if (front==nullptr){
        Term *f = new Term(expon, coeff, nullptr);
        front=f;
     }
  else {
     Term *p = new Term(expon, coeff, nullptr);
     Term *q;
     q=front;
     Term *temp;
     temp=q;
   while ( q->exponent> p->exponent && q->next !=nullptr){
        temp=q;
        q=q->next;
     }
     if (q->exponent> p->exponent && q->next ==nullptr ){
        p->next=nullptr;
        q->next=p;

     }
    else if(q->exponent==p->exponent){
        int sum = q->coefficient+p->coefficient;
        if (sum == 0){
                if(front == q){
                 front=front->next;
                }
        else temp->next=q->next;
        
        }
        else q->coefficient=sum;

    }
    else if (q->exponent < p->exponent){
            p->next=q;

           if (temp==front && q==front)
            front=p;
           else temp->next=p;

     }
  }



    }


 double Polynomial:: evaluate(double x){
   double result = 0;
   for(Term *r = front; r != nullptr; r = r->next)
    result += r->coefficient*pow(x, r->exponent);
   return result;  
 };

Polynomial operator+ (const Polynomial &p, const Polynomial &q){
  Polynomial final_pol;
  final_pol = p;
  for(auto g = q.begin(); g != q.end(); ++g)
    final_pol.addTerm(g.getItExp(), g.getItCoef());
  
  return final_pol;
 };

 Polynomial operator* (const Polynomial &p, const Polynomial &q){
   Polynomial final_pol;
   for(auto k = p.begin(); k != p.end(); ++k){
     for(auto m = q.begin(); m != q.end(); ++m){
       int Exp = k.getItExp()+m.getItExp();
       int Coef = k.getItCoef()*m.getItCoef();
       final_pol.addTerm(Exp, Coef);
       }
   }
   return final_pol;
 };

 ostream & operator << (ostream &out, const Polynomial &p){
   if(p.begin() == p.end())
     out << 0 << endl;
   else{
     for(auto l = p.begin(); l != p.end(); ++l){
       if(l != p.begin()){
         if((l.getItCoef() > 0) && (l.getItCoef() != 1))
          out << " + " << l.getItCoef();
         else if((l.getItCoef() != -1) && (l.getItCoef() != 1))
           out << " - " << abs(l.getItCoef());
         else{
           if(l.getItCoef() == 1)
            out << " + ";
           else if(l.getItCoef() == -1)
            out << " - "; 
         }

       if((l.getItExp() != 0) && (l.getItExp() != 1))
         out << "x^" << l.getItExp();
       else if(l.getItExp() == 1)
         out << "x";     
       else if((l.getItExp() == 0) && (abs(l.getItCoef()) == 1))
         out << abs(l.getItCoef());
       }
      else{
       if((l.getItCoef() > 0) && (l.getItCoef() != 1))
         out << l.getItCoef();
       else if((l.getItCoef() != -1) && (l.getItCoef() != 1))  out << '-' << l.getItCoef();
       else{
         if(l.getItCoef() != 1)
            out << '-';
         } 

       if((l.getItExp() != 0) && (l.getItExp() != 1))
         out << "x^" << l.getItExp();
       else if(l.getItExp() == 1)
         out << "x"; 
       else if((l.getItExp() == 0) && (abs(l.getItCoef()) == 1))
         out << abs(l.getItCoef());
     }
     
     }
   } 
   return out;
   
 };

int main() {
 Polynomial p; // 0
 p.addTerm(1, 3); // 3x
 p.addTerm(2, 1); // x^2
 p.addTerm(0, -1); // -1

 Polynomial q(p); // x^2 + 3x - 1
 q.addTerm(1, -3); // -3x

 cout << "P(x) = " << p << endl;
 cout << "P(1) = " << p.evaluate(1) << endl;
 cout << "Q(x) = " << q << endl;
 cout << "Q(1) = " << q.evaluate(1) << endl;
 cout << "(P+Q)(x) = " << p+q << endl;
 cout << "(P*Q)(x) = " << p*q << endl;


}