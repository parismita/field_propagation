//
// class G4VChordFinder
//
// Class description:
//
// It is a base class for ChordFinder classes
//
// History:
// - Created: D. Sorokin
// --------------------------------------------------------------------

#ifndef G4VChordFinder_HH
#define G4VChordFinder_HH

#include "G4Types.hh"
#include "G4FieldTrack.hh"
#include "G4Mag_EqRhs.hh"
#include "G4EquationOfMotion.hh"
#include "G4MagIntegratorStepper.hh"

#include "G4VIntegrationDriver.hh"


class G4VChordFinder
{
public:

    G4VChordFinder(G4VIntegrationDriver* pIntDriver, G4int statisticsVerbosity);

    virtual ~G4VChordFinder();

    G4VChordFinder(const G4VChordFinder&);

    G4VChordFinder& operator = ( G4VChordFinder&);

    /*  Uses ODE solver's driver to find the endpoint that satisfies
     *  the chord criterion: that d_chord < delta_chord
     *  returns Length of Step taken.
     * */
    virtual G4double  AdvanceChordLimited(G4FieldTrack& trackCurrent, G4double stepLen, G4double eps) = 0;


    G4FieldTrack ApproxCurvePointS(const G4FieldTrack&  curveAPointVelocity,
                                           const G4FieldTrack&  curveBPointVelocity,
                                           const G4FieldTrack&  ApproxCurveV,
                                           const G4ThreeVector& currentEPoint,
                                           const G4ThreeVector& currentFPoint,
                                           const G4ThreeVector& PointG,
                                           G4bool first,  G4double epsStep);

    G4FieldTrack ApproxCurvePointV(const G4FieldTrack& trackPointA,
                                           const G4FieldTrack& trackPointB,
                                           const G4ThreeVector& pointE,
                                           G4double epsStep);

    /*
     * Resets internal state of G4ChordFinder
     * this is needed for drivers with interpolation of
     * adams methods
     * */
    virtual void reset() = 0;

    //this function is called by ApproxCurvePointS, ApproxCurvePointV
    virtual G4bool DoStepForIntersection(G4FieldTrack&  track, G4double stepLen, G4double eps) = 0;


    inline G4EquationOfMotion* GetEquationOfMotion();
    inline void SetEquationOfMotion(G4EquationOfMotion* newEquation);

    inline void SetIntegrationDriver(G4VIntegrationDriver* IntDriver);
    inline G4VIntegrationDriver* GetIntegrationDriver();

    //inline function definition
    inline G4double InvParabolic( const G4double xa, const G4double ya,
                                  const G4double xb, const G4double yb,
                                  const G4double xc, const G4double yc );




    inline G4double  GetDeltaChord() const;

    inline void SetDeltaChord(G4double newDelta);

    // Clear internal state (last step estimate)
    inline void ResetStepEstimate();

    // Get statistics about number of calls & trials in FindNextChord
    inline G4int GetNoCalls();
    inline G4int GetNoTrials();        // Total number of trials
    inline G4int GetNoMaxTrials();     // Maximum # of trials for one call

    // Set verbosity and return old value
    inline G4int SetVerbose( G4int newvalue=1);

    inline G4int GetVerbose() const;

    // Parameter for  performance ... change with great care
    inline void SetFirstFraction(G4double newVal);
    inline G4double GetLastStepEstimateUnc();
    inline void     SetLastStepEstimateUnc( G4double stepEst );


    //   Printing for monitoring ...
    inline   G4double GetFirstFraction();         // Originally 0.999
    inline   G4double GetFractionLast();          // Originally 1.000
    inline   G4double GetFractionNextEstimate();  // Originally 0.980
    inline   G4double GetMultipleRadius();        // No original value

    inline G4bool AcceptableMissDist(G4double dChordStep) const;

    /*Accumulate the basic statistics
     *other specialised ones must be kept by derived classes
     * */
    inline void AccumulateStatistics( G4int noTrials );


    // A report with the above -- and possibly other stats
    void PrintStatistics();

    // Parameters for  performance ... change with great care
    void SetFractions_Last_Next( G4double fractLast= 0.90,
                                 G4double fractNext= 0.95 );

    void TestChordPrint(G4int noTrials,
                        G4int    lastStepTrial,
                        G4double dChordStep,
                        G4double nextStepTrial );



    /* Does one step not larger than stepLen with accuracy eps
     * returnes hdid
     * */
    //virtual G4double OneGoodStep(G4FieldTrack&  track, G4double stepLen, G4double eps) = 0;

    /* Returns step size, which satisfied chord rule: chord < fDeltaChord
     * Moves track
     * */
    //virtual G4double FindNextChord(G4FieldTrack& track, G4double stepMax) = 0;


    G4double NewStep( G4double stepTrialOld,
                      G4double dChordStep,     // Current dchord estimate
                      G4double& stepEstimate_Unconstrained );


    void PrintDchordTrial(G4int noTrials,
                          G4double  stepTrial,
                          G4double  oldStepTrial,
                          G4double  dChordStep);



private:

     G4VIntegrationDriver* fIntDriver;

     // verbosity level
     G4int fverb;

     // SET in G4VChordFinderRefined.cc = 0.25 mm
     const G4double fDefaultDeltaChord;

     //  PARAMETERS
     //  ---------------------
     G4double  fDeltaChord;               //  Maximum miss distance
     //    Internal parameters
     G4double  fFirstFraction, fFractionLast, fFractionNextEstimate;
     G4double  fMultipleRadius;
     G4int     fStatsVerbose;  // if > 0, print Statistics in destructor


     //  STATE information
     G4double    fLastStepEstimate_Unconstrained;
       //  State information for efficiency

     // For Statistics
     // -- G4int   fNoTrials, fNoCalls;
     G4int   fTotalNoTrials_FNC,  fNoCalls_FNC, fmaxTrials_FNC; // fnoTimesMaxTrFNC;
};

//inline function definition
#include "G4VChordFinder.icc"

#endif
