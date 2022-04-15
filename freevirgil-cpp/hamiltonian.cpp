#include <cmath>
using namespace std;

class hamiltonian {
    /*
    Class for Hamiltonian

    .. math::
        H = -J\\sum_{\\left<ij\\right>} \\sigma_i\\sigma_j + \\mu\\sum_i\\sigma_i
    */
    private:

        double J;
        double mu;

    public: 

        hamiltonian(double, double);
        double E(spin_conf);
        double avg(spin_conf, int, 
			double&, double&, double&, double&);
};

hamiltonian::hamiltonian(double J=-2.0, double mu=1.0) {
    /*
    Constructor

    Parameters
    ----------
    J : float, optional
        Coupling strength
    mu : float, optional
        Chemical potential
    */ 
    this->J = J;
    this->mu = mu;
}
        
double hamiltonian::E(spin_conf spin) {
    /*
    Energy of configuration `spin`

    .. math::
            E = \\left<\\hat{H}\\right> 

    Parameters
    ----------
    spin : :class:`spin_conf`
        Spin configuration

    Returns
    -------
    energy : float
        Energy of configuration
    */
    int sum1 = 0;
    int sum2 = 0;
    for (int i = 0; i < spin.sites; i++)
    {
        sum1 += spin.config[i] * spin.config[i+1];
        sum2 += spin.config[i];
        // Periodic boundary conditions
        if (i == spin.sites-2)
        {
            sum1 += spin.config[i+1] * spin.config[0];
            sum2 += spin.config[i+1];
        }
    }
    return (-(this->J) * sum1) + (this->mu * sum2);      
}

void hamiltonian::avg(spin_conf conf, int T, 
    double& E, double& M, double& HC, double& MS)
{
    /*
    Exact average values

    Parameters
    ----------
    spin : :class:`spin_conf`
        Spin configuration
    T : int
        Temperature
    
    Returns 
    -------
    E : float
        Energy of configuration 
    M : float
        Magnetization of configuration
    HC : float
        Heat capacity
    MS : float
        Magnetic susceptability
    */     
     
    E = M = HC = MS = 0.0;
    double Z = 0.0,
           Zi = 0.0,
           Ei = 0.0,
           Mi = 0.0,
           EE = 0.0,
           MM = 0.0;
    
    for (int i=0; i < conf.dim; i++)
    {               
        // generate each possible configuration
        conf.dec_conf(i);
        
        Ei = this->E(conf);
        Zi = exp( -Ei/T );
        E += Ei * Zi;
        EE += Ei * Ei * Zi;
        
        Mi = conf.M();
        M += Mi * Zi;
        MM += Mi * Mi * Zi;
        Z += Zi;
          
    E = E/Z;
    M = M/Z;
    EE = EE/Z;
    MM = MM/Z;

    HC = (EE - E*E) / (T*T);
    MS = (MM - M*M) / T;
}
