# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions 103X_dataRun2_Prompt_v2 -s RAW2DIGI,L1Reco,RECO --process reRECO -n 30 --data --era Run2_2018_pp_on_AA --eventcontent AOD --runUnscheduled --scenario pp --datatier AOD --repacked --filein file:out.root --fileout file:step2.root --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process("compressJets")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    # fileNames = cms.untracked.vstring('file:/eos/cms/store/hidata/HIRun2018A/HIMinimumBias5/RAW/v1/000/326/573/00000/C4CDB29C-62C9-AF4D-B2A3-5D9793BE3832.root'),
    fileNames = cms.untracked.vstring('file:/eos/cms/store/group/phys_heavyions/chenyi/Samples/store/himc/HINPbPbAutumn18DR/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/AODSIM/mva98_103X_upgrade2018_realistic_HI_v11-v1/40000/43A47121-AC67-0A41-8ACA-F5295E69E562.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:30'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

process.TFileService = cms.Service("TFileService",
    fileName=cms.string("./BasicCompressor/test/tree/compressed_p4.root"))

process.BasicCompressor = cms.EDProducer("BasicCompressor",
	inputPFJets = cms.InputTag("kt4PFJetsForRho")
)

process.BasicCompressorDump = cms.EDAnalyzer("BasicCompressorDump",
    inputPFJets = cms.InputTag("kt4PFJetsForRho"),
    inputPFJetsCompressed = cms.InputTag("kt4PFJetsForRho")
)

process.outputCompress = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test_compress.root'),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(8),
    eventAutoFlushCompressedSize = cms.untracked.int32(31457280),
    outputCommands = cms.untracked.vstring('drop *',
    #   "keep recoPFJets_kt4PFJetsForRho_*_*",
    #   "keep recoPFJets_BasicCompressor*_*_*",
      "keep recoPFJets_*_*_*",
      "keep *_BasicCompressor_*_*"
    )
)

process.basicCompressor_step = cms.Path(process.BasicCompressor)
process.analyzer_step = cms.Path(process.BasicCompressorDump)

process.output = cms.EndPath(process.outputCompress)
process.schedule = cms.Schedule(process.basicCompressor_step, process.analyzer_step, process.output)