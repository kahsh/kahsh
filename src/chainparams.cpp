// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2019 The Kahsh Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    ( 0, uint256("0x000004fac39621b6b0a0abb110ad82ae384ac4a7fac2b1b5a79aa8521e771a4f"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1557431124, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
    //   (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x000002f01a6a392d77b52ee5d0bdd2e9f605cd5f4cdf6ce8f6594743a65fde00"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1557434098,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x2b6d7f1011d27fd665d86d6e905053c4ed8cbcd1df10e33aac1fe1885ddd41d3"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1557434250,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

bool FindGenesisBlock(CBlock &block)
{
    uint256 bnTarget;
    bnTarget.SetCompact(block.nBits);

    for (uint32_t nNonce = 0; nNonce < UINT32_MAX; nNonce++) {
        block.nNonce = nNonce;

        uint256 hash = block.GetHash();
        if (hash <= bnTarget){
            printf("genesis block.nNonce = %u \n", block.nNonce);
            printf("genesis block.GetHash = %s\n", hash.ToString().c_str());
            printf("genesis block.hashMerkleRoot = %s\n", block.hashMerkleRoot.ToString().c_str());

            return true;
        }
    }

    // This is very unlikely to happen as we start with a very low difficulty.
    assert(false);
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xad;
        pchMessageStart[2] = 0x8d;
        pchMessageStart[3] = 0xfc;
        vAlertPubKey = ParseHex("04e0a7aad398038c09ef4fd31416a1553dded4901017efbcc363f291d1edcb1c6e88c82e307cc1ebcc79fce05ae0ed8960b90b5fcc9c3d022584ccb6881716d142");
        nDefaultPort = 52210;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Kahsh starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Kahsh: 1 day
        nTargetSpacing = 1 * 60;  // Kahsh: 1 minute
        nMaturity = 174;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 21000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 400;
        nModifierUpdateBlock = 0;
        nZerocoinStartHeight = 30;
        nZerocoinStartTime = 1652659200;
        nBlockEnforceSerialRange = 0; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = INT_MAX - 1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = INT_MAX - 1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = INT_MAX - 1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 0; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN;
        nBlockZerocoinV2 = nZerocoinStartHeight;
        nEnforceNewSporkKey = 1542193313;
        nRejectOldSporkKey = nEnforceNewSporkKey;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "New York Times May 16 2019 It’s Time to Break Up Facebook";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 250 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0429e52c5d3b7825c014d69c378a6e3fe7d6d60a4cd432dd70fe041f5ed4c52cab7e4f353cd07cec3473d6f4ee209565e16b3b0c68e5f62951fa43f87a3b735502") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1557431124;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 11555;

        hashGenesisBlock = genesis.GetHash();
        // FindGenesisBlock(genesis);
        assert(hashGenesisBlock == uint256("0x000004fac39621b6b0a0abb110ad82ae384ac4a7fac2b1b5a79aa8521e771a4f"));
        assert(genesis.hashMerkleRoot == uint256("0x2419010ffd3d2c65d091afbaff796197ea7b1321f7690be5a1c943c0113dfa8c"));

        // vSeeds.push_back(CDNSSeedData("kahsh.io", "xdhseed.cryptertech.io"));
        // vSeeds.push_back(CDNSSeedData("kahsh.io", "xdhseed2.cryptertech.io"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 107);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 134);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 95);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x22)(0x08)(0x06)(0x02).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x22)(0x07)(0x06)(0x00).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x09)(0x02)(0x02).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04104074b42202fcb47644522bbb8a63e388a84d33692a9f2a8823286c663788047031d1c817d043c3dc02d8a21fe94d798b0492d0181658a9001737ea2e6320fb";
        strSporkKeyOld = "04104074b42202fcb47644522bbb8a63e388a84d33692a9f2a8823286c663788047031d1c817d043c3dc02d8a21fe94d798b0492d0181658a9001737ea2e6320fb";
        strObfuscationPoolDummyAddress = "dVJsSAz71SwPj9ApcU3gUTSKDcgY5pSG1W";
        nStartMasternodePayments = 1557964800;

        /** Zerocoin */
        zerocoinModulus = "43576052031536534974087126771477540874776068735756049793442594061574276669938457998755994398476021"
                "795030564757324158256540965678312098953769574013581560766234774267661608068134114816266094160272604326733173"
                "479800555359291397176614410393075621316487512261671229102009732039578451373511087272512069861142667778062039"
                "649038233698043690728967120188434234386930578115299235860171468178079299084777967372105651716413259080023925"
                "707043193802211315999861250720041971107415418755470665968645227271317543707570402065871857295511143774933748"
                "937503955077547233512155343603579412119043043204866747475333130841452645261835055390175197387724700033960061"
                "441923178337150173575121284567645477194266064764723867886217312416487694858985085679583430617376020463127172"
                "059877016414734206869424304098152299880227982394705203618939716964998376892168035051851195013776034254709958"
                "23525108825797724241022595983834447217912092980344779688031823708494921";

        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zxdh to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee

        /** Developer block reward */
        strDevBlockRewardAddress = "k8CGuAr8ycm7JopanafxAoAazWrtNF6c7A";

        /** Staking Requirements (spork activated) */
        nStakeMinConfirmations = 720; // Required number of confirmations
        nStakeMinAmount = 20 * COIN; // Minimum required staking amount
        nStakeMinAge = 60 * 60 * 12; // 12 hours
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xaf;
        pchMessageStart[1] = 0x65;
        pchMessageStart[2] = 0x84;
        pchMessageStart[3] = 0x7b;
        vAlertPubKey = ParseHex("04b5a88ccde32007ebdda433684f7c658d22c44449a129a1b1cba780a59b7846e20b4341e3c782d475ca6305a4807dfa17c8c68d530cabe501054cdcf8025861f1");
        nDefaultPort = 52220;
        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Kahsh: 1 day
        nTargetSpacing = 1 * 60;  // Kahsh: 1 minute
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nMaxMoneyOut = 43199500 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        nModifierUpdateBlock = 0;
        nZerocoinStartHeight = 30;
        nZerocoinStartTime = 1652659200;
        nBlockEnforceSerialRange = 0; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = INT_MAX - 1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = INT_MAX - 1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = INT_MAX - 1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 0; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN;
        nBlockZerocoinV2 = nZerocoinStartHeight;
        nEnforceNewSporkKey = 1543057352;
        nRejectOldSporkKey = nEnforceNewSporkKey;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1557434098;
        genesis.nNonce = 2404222;

        hashGenesisBlock = genesis.GetHash();
        //FindGenesisBlock(genesis);
        assert(hashGenesisBlock == uint256("0x000002f01a6a392d77b52ee5d0bdd2e9f605cd5f4cdf6ce8f6594743a65fde00"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 125); // Testnet kahsh addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 114);  // Testnet kahsh script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 200);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet pivx BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x18)(0x04)(0x05)(0x00).convert_to_container<std::vector<unsigned char> >();
        // Testnet pivx BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x18)(0x08)(0x09)(0x00).convert_to_container<std::vector<unsigned char> >();
        // Testnet pivx BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x02)(0x01)(0x02).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04b5a88ccde32007ebdda433684f7c658d22c44449a129a1b1cba780a59b7846e20b4341e3c782d475ca6305a4807dfa17c8c68d530cabe501054cdcf8025861f1";
        strSporkKeyOld = "04b5a88ccde32007ebdda433684f7c658d22c44449a129a1b1cba780a59b7846e20b4341e3c782d475ca6305a4807dfa17c8c68d530cabe501054cdcf8025861f1";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet

        /** Developer block reward */
        strDevBlockRewardAddress = "k8CGuAr8ycm7JopanafxAoAazWrtNF6c7A";

        /** Staking Requirements (spork activated) */
        nStakeMinConfirmations = 470; // Required number of confirmations
        nStakeMinAmount = 5 * COIN; // Minimum required staking amount
        nStakeMinAge = 60 * 60 * 4; // 4 hours
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xf0;
        pchMessageStart[1] = 0xa2;
        pchMessageStart[2] = 0xe5;
        pchMessageStart[3] = 0x83;
        nDefaultPort = 28680;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Kahsh: 1 day
        nTargetSpacing = 1 * 60;        // Kahsh: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nLastPOWBlock = 250;
        nMaturity = 100;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 0; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 300;
        nBlockZerocoinV2 = 300;
        nZerocoinStartTime = 1652659200;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint

        //! Modify the regtest genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1557434250;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 1;

        hashGenesisBlock = genesis.GetHash();
        // FindGenesisBlock(genesis);
        assert(hashGenesisBlock == uint256("0x2b6d7f1011d27fd665d86d6e905053c4ed8cbcd1df10e33aac1fe1885ddd41d3"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 38680;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
