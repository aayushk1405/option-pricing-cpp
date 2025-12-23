#include <iostream>
#include <cmath>
#include <random>
#include<algorithm>
#include<vector>
using namespace std;

class Option{
public:
    virtual ~Option()= default;
    virtual double payoff(double ST) const=0;
    virtual bool call() const=0;
    virtual double strike() const=0;
};

class EuropeanCall: public Option{
    double K;
public:
    explicit EuropeanCall(double strike): K(strike) {}
    double payoff(double ST) const override {
        return max(ST-K,0.0);
    }
    bool call() const override {return true;}
    double strike() const override {return K;}
};

class EuropeanPut: public Option{
    double K;
public:
    explicit EuropeanPut(double strike){
        this->K= strike;
    }
    double payoff(double ST) const override {
        return max(K-ST,0.0);
    }
    bool call() const override {return false; }
    double strike() const override {return K;}

};

struct Parameters{
    double S; //current stock price
    double sigma; //volatality
    double r; //risk-free rate
    double T; //time to maturity/expiry
};

class RNG{
private:
    mt19937 gen;
    normal_distribution<double> rv;

public:
    RNG(): gen(random_device{}()), rv(0.0,1.0) {}
    double sample(){
        return rv(gen);
    }

};

class MonteCarloprice{
public:
    double mc_price(const Option& option,const Parameters& p,RNG& rng,int n) const {
        double total_payoff=0.0;

        for(int i=0;i<n;i++){
            double z= rng.sample();
            double ST= p.S*exp((p.r- 0.5*p.sigma*p.sigma)*p.T + p.sigma*sqrt(p.T)*z);
            total_payoff+= option.payoff(ST);
        }

        double price= exp(-p.r*p.T)*(total_payoff/n);
        return price;
    }
};

class BlackScholesprice{
    double normal_cdf(double x) const {
        return 0.5*(1.0+ erf(x/sqrt(2.0)));
    }
    double d1(const Option& option,const Parameters& p) const{
        return (log(p.S/option.strike())+ (p.r+ 0.5*p.sigma*p.sigma)*p.T)/(p.sigma*sqrt(p.T));
    }
    double d2(const Option& option,const Parameters& p) const{
        return d1(option,p) - ((p.sigma)*sqrt(p.T)) ;
    }
public:
    double bsprice(const Option& option,const Parameters& p) const {
        double k1= d1(option,p);
        double k2= d2(option,p);

        if(option.call()){
            return p.S*normal_cdf(k1) - option.strike()*exp(-p.r*p.T)*normal_cdf(k2);
        }else{
            return option.strike()*exp(-p.r*p.T)*normal_cdf(-k2)- p.S*normal_cdf(-k1);
        }
    }

    double delta(const Option& option,const Parameters& p) const {
        double a1= d1(option,p);
        double n1= normal_cdf(a1);
        
        if(option.call()){
            return n1;
        }else{
            return n1-1.0;
        }

    }
};

class BinomialTree{
public:
    double price(const Option& option,const Parameters& p,int N) const {
        double dt= p.T/N;
        double u= exp(p.sigma*sqrt(dt));
        double d= 1.0/u;
        double disc= exp(-p.r*dt);
        double prob= (exp(p.r*dt)-d)/(u-d);

        vector<double> values(N+1);
        for(int i=0;i<=N;i++){
            double ST= p.S*pow(u,i)*pow(d,N-i);
            values[i]=option.payoff(ST);
        }

        for(int i=N-1;i>=0;i--){
            for(int j=0;j<=i;j++){
                values[j]=disc*(prob*values[j+1]+(1.0-prob)*values[j]);
            }
        }
        return values[0];
    }

};

int main(){

    Parameters p;
    p.S   = 100.0;   // stock price
    p.sigma = 0.20;    // volatility (20%)
    p.r     = 0.05;    // risk-free rate (5%)
    p.T     = 1.0;     // 1 year to maturity

    double K = 100.0;
    EuropeanCall call(K);
    EuropeanPut  put(K);

    RNG rng;
    MonteCarloprice m;
    BlackScholesprice bs;
    BinomialTree bi;

    int n = 1000000;
    int N = 200; 

    double call_mc = m.mc_price(call, p, rng,n);
    double put_mc  = m.mc_price(put,  p, rng, n);

    double call_bs = bs.bsprice(call,p);
    double put_bs  = bs.bsprice(put,p);

    double call_delta= bs.delta(call,p);
    double put_delta= bs.delta(put,p);

    double call_bi = bi.price(call,p, N);
    double put_bi  = bi.price(put,p,N);

    cout << "European Call Prices\n";
    cout << "Monte Carlo : " << call_mc << "\n";
    cout << "BlackScholes: " << call_bs << "\n";
    cout << "Binomial    : " << call_bi << "\n\n";

    cout << "European Put Prices\n";
    cout << "Monte Carlo : " << put_mc << "\n";
    cout << "BlackScholes: " << put_bs << "\n";
    cout << "Binomial    : " << put_bi << "\n";

    cout << "Delta for call:" << call_delta << endl;
    cout << "Delta for put:" << put_delta << endl;

    return 0;
}