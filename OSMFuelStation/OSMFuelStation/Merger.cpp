#include "Merger.h"
#include "Utils.h"

const double Merger::OFFSET_LIMIT = 500; // in meters

Merger::Merger(void)
{
}


Merger::~Merger(void)
{
}

vector<ComparedPair*> Merger::CompareLists(const vector<FuelStation*>& original, const vector<FuelStation*>& applicants, RegionStats& stats) const {
    vector<ComparedPair*> pairs;
    ComparedPair* p_pair;
    for (std::vector<FuelStation*>::const_iterator it = original.cbegin() ; it != original.cend(); ++it) {
        ComparedPair pair;
        for (std::vector<FuelStation*>::const_iterator jt = applicants.cbegin() ; jt != applicants.cend() && pair.mState == ComparedPair::NOT_PRESENT; ++jt) {
            pair = ComparePair(*it, *jt);
            if (pair.mState!=ComparedPair::NOT_PRESENT) {
                p_pair = new ComparedPair(pair);
                pairs.push_back(p_pair);
                stats.states[p_pair->mState]++;
            }
        }
        if (pair.mState == ComparedPair::NOT_PRESENT) {
            ComparedPair* p_pair = new ComparedPair(*(*it));
            pairs.push_back(p_pair);
            stats.states[p_pair->mState]++;
        }
    }
    return pairs;
}

ComparedPair Merger::ComparePair(const FuelStation& original, const FuelStation& applicant) const {
    return ComparePair(&original, &applicant);
}

ComparedPair Merger::ComparePair(const FuelStation* original, const FuelStation* applicant) const {
    ComparedPair pair;
    double offset = 0.;
    int match = 0;
    if (original->mLocalRef!=-1 && original->mLocalRef == applicant->mLocalRef) match|=Match::LOCALREF;
    if (!original->mBrand.empty() && original->mBrand == applicant->mBrand) match|=Match::BRAND;
    if (!original->mOperator.empty() && original->mOperator == applicant->mOperator) match|=Match::OPERATOR;
    if (!original->mName.empty() && original->mName == applicant->mName) match|=Match::NAME;
    // kind of hack, to improve search
    if (!original->mBrand.empty() && original->mBrand == applicant->mName) match|=Match::BRAND;
    if (!original->mBrand.empty() && original->mBrand == applicant->mOperator) match|=Match::BRAND;

    if (original->mLat != Utils::INVALID_LATLON || original->mLon != Utils::INVALID_LATLON || 
        applicant->mLat != Utils::INVALID_LATLON || applicant->mLon != Utils::INVALID_LATLON) {
        offset = Utils::Distance(original->mLat, original->mLon, applicant->mLat, applicant->mLon)*1000;
        if (offset<OFFSET_LIMIT) match|=Match::COORDS;
    }
    if (original->mFuelTypes == applicant->mFuelTypes) match|=Match::FUEL;
    if (match == ST_VALID) {
        pair.mState = ComparedPair::ValidationState::VALID;
        pair.SetOriginal(*original);
        pair.SetApplicant(*applicant);
        pair.mOffset = offset;
    }
    else if ((match & Match::LOCALREF) && ((match & Match::BRAND) || (match & Match::OPERATOR)) && (match & Match::COORDS)) {
        pair.mState = ComparedPair::ValidationState::LOOKS_SIMILAR;
        pair.SetOriginal(*original);
        pair.SetApplicant(*applicant);
        pair.mOffset = offset;
    }
    else if (((match & Match::COORDS) || (match & Match::LOCALREF)) && ((match & Match::BRAND) || (match & Match::OPERATOR))) {
        pair.mState = ComparedPair::ValidationState::INVALID;
        pair.SetOriginal(*original);
        pair.SetApplicant(*applicant);
        pair.mOffset = offset;
    }
    return pair;
}