# Option Pricing Engine in C++

This project implements a modular option pricing engine in C++ for European options.  
It compares analytical and numerical pricing approaches using Black–Scholes, Monte Carlo simulation, and Binomial Tree models.  
The design emphasizes clean abstractions, polymorphism, and numerical convergence.

---
## Pricing Models

- **Black–Scholes Model**
  - Closed-form analytical pricing for European options
  - Used as a reference benchmark

- **Monte Carlo Simulation**
  - Risk-neutral simulation of terminal stock prices
  - Demonstrates convergence to analytical prices as sample size increases

- **Binomial Tree Model**
  - Discrete-time numerical approximation
  - Converges to Black–Scholes as tree depth increases

---
## Options Supported

- European Call Option
- European Put Option
---

## Greeks

- **Delta**
  - Implemented analytically using the Black–Scholes model
  - Verified expected behavior for call and put options

---

## Design & Architecture

- Abstract `Option` interface with polymorphic payoff evaluation
- Stateless pricing engines for each model
- Shared market parameters passed explicitly to avoid global state
- Clean separation between financial instruments and pricing logic

---

## Sample Parameters

S-> Current Stock price
K-> Strike price
σ-> volatality
r-> risk-free rate
T-> time to maturity/expiry

| Parameter | Value |
|-----------|-------|
| S         | 100   |
| K         | 100   |
| σ         | 20%   |
| r         | 5%    |
| T         | 1 yr  |

---
## Sample Output

**European Call:**
Monte Carlo: 10.43
Black–Scholes: 10.45
Binomial Tree: 10.44

**European Put:**
Monte Carlo: 5.57
Black–Scholes: 5.57
Binomial Tree: 5.56


**Deltas:**
Call Delta: 0.637
Put Delta: -0.363


* Monte Carlo prices exhibit variance for small sample sizes and converge to Black–Scholes as the number of simulations increases.  
* Binomial Tree prices converge to the analytical solution as the number of steps increases.

---
## Build & Run

```bash
# Compile
g++ -std=c++17 -O2 main.cpp -o Option_pricing

# Run
./Option_pricing

```md

- Implemented in a single translation unit for simplicity and ease of review
- Architecture is designed to scale naturally to multi-file projects
- Emphasis placed on correctness, numerical convergence, and clean abstractions

##Mathematical Formulas for Price Computation

Where:
- $S$ is the current stock price
- $K$ is the strike price
- $r$ is the risk-free interest rate
- $T$ is time to maturity
- $N(\cdot)$ is the standard normal CDF

### Monte Carlo Simulation

Under the risk-neutral measure, the terminal stock price is simulated as:

$$
S_T = S \exp\left((r - \frac{1}{2}\sigma^2)T + \sigma \sqrt{T} Z\right),
\quad Z \sim \mathcal{N}(0,1)
$$

### Black–Scholes Formula

The Black–Scholes model provides a closed-form solution for pricing European options.

$$
C = S N(d_1) - K e^{-rT} N(d_2)
$$

$$
P = K e^{-rT} N(-d_2) - S N(-d_1)
$$

Where:
$$
d_1 = \frac{\ln(S / K) + (r + \frac{1}{2}\sigma^2)T}{\sigma \sqrt{T}}
$$

$$
d_2 = d_1 - \sigma \sqrt{T}
$$

### Binomial Tree Pricing

Option values are computed via backward induction(for both American and European Options):

$$
V_t^{(j)} = e^{-r\Delta t} \left[p V_{t+\Delta t}^{(j+1)} + (1-p)V_{t+\Delta t}^{(j)}\right]
$$





