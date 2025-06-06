#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import pydiag as yd
import pydiag.ensemble as yde
from collections import OrderedDict

nsitess = [16, 20]
J=1.00
Jd=1.00

temperatures = np.linspace(0.01, 1.0, 100)

for nsites in nsitess:
    # define ensemble of quantum numbers with degeneracies (qn, deg)
    nups = [(nup, 1) if nup == nsites // 2 else (nup, 2) for nup in range(nsites//2+1)]
    if nsites == 16:
        ks = [("Gamma.D4.A1", 1), ("Gamma.D4.A2", 1), ("Gamma.D4.B1", 1),
              ("Gamma.D4.B2", 1), ("Gamma.D4.E", 2), ("M.D4.A1", 1),
              ("M.D4.A2", 1), ("M.D4.B1", 1), ("M.D4.B2", 1), ("M.D4.E", 2),
              ("Sigma.D1.A", 4), ("Sigma.D1.B", 4), ("X.D2.A1", 2),
              ("X.D2.A2", 2), ("X.D2.B1", 2), ("X.D2.B2", 2)]
        
    if nsites == 20:
        ks = [("Gamma.C4.A", 1), ("Gamma.C4.B", 1), ("Gamma.C4.Ea", 1),
              ("Gamma.C4.Eb", 1), ("M.C4.A", 1), ("M.C4.B", 1), ("M.C4.Ea", 1),
              ("M.C4.Eb", 1), ("None0.C1.A", 4), ("None1.C1.A", 4)]

    ensemble = yde.Ensemble(nups, ks)
    directory = "outfiles/".format(nsites)
    regex = "outfile.checkerboard.{}.J.{:.2f}.Jd.{:.2f}.nup.(.*).k.(.*).h5".format(nsites, J, Jd)

    data = yd.read_h5_data(directory, regex, tags=["Eigenvalues"])
    eigs = yde.Array(ensemble, data, tag="Eigenvalues").flatten()
    e0 = eigs.min().min()

    specheats = []
    output = np.zeros((len(temperatures), 5))

    for Tidx, T in enumerate(temperatures):
        print("T =", T)
        beta = 1 / T
        boltzmann = yde.exp(-beta * (eigs - e0))
        partition = yde.sum(boltzmann, degeneracies=True)
        energy = yde.sum(eigs * boltzmann, degeneracies=True)
        energy2 = yde.sum(eigs * eigs * boltzmann, degeneracies=True)
        specheat = (energy2/partition - (energy/partition)**2) / T
        output[Tidx, 0] = T
        output[Tidx, 1] = partition
        output[Tidx, 2] = energy / partition
        output[Tidx, 3] = energy2 / partition
        output[Tidx, 4] = (energy2/partition - (energy/partition)**2) / (T**2)

    np.savetxt("moments/moments.checkerboard.{}.J.{:.2f}.Jd.{:.2f}.txt".format(nsites, J, Jd), output, 
               header = "{:<24} {:<24} {:<24} {:<24} {:<24}".format("T", "Z", "E", "E2", "C"))
