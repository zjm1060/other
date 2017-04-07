package bitc.sz.wzEnv.service.impl;

import bitc.sz.wzEnv.dao.ModelInputDao;
import bitc.sz.wzEnv.dao.impl.ExecuteExeSZPJDaoImpl;
import bitc.sz.wzEnv.service.ModelInput;
import org.apache.commons.lang.StringUtils;
import org.apache.http.NameValuePair;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.*;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/7/4.
 */
@Service
public class ModelInputImpl implements ModelInput {
    @Autowired
    private ModelInputDao dao;
    @Autowired
    private ExecuteExeSZPJDaoImpl szpjDao;

    @Override
    public List<Map<String, String>> getZhiliuListData(String water_body_id) throws Exception {
        return dao.getZhiliuListData(water_body_id);
    }

    @Override
    public List<Map<String, String>> getBianjieListData(String water_body_id, String plan_id) throws Exception {
        return dao.getBianjieListData(water_body_id, plan_id);
    }

    @Override
    public List<Map<String, String>> getHedaoListData() throws Exception {
        return dao.getHedaoListData();
    }

    @Override
    public List<Map<String, String>> getDuanMianListData(int hedao) throws Exception {
        return dao.getDuanMianListData(hedao);
    }

    @Override
    public List<Map<String, String>> getHeDuanListData(String water_body_id) throws Exception {
        return dao.getHeDuanListData(water_body_id);
    }

    @Override
    public List<Map<String, Object>> getZhaMenListDate(String water_body_id) throws Exception {
        return dao.getZhaMenListDate(water_body_id);
    }

    @Override
    public List<Map<String, String>> getZhaMenNameAndDmId() throws Exception {
        return dao.getZhaMenNameAndDmId();
    }

    @Override
    public List<Map<String, Object>> getShuiKuListDate(String water_body_id) throws Exception {
        return dao.getShuiKuListDate(water_body_id);
    }
        @Override
    public boolean writeEfdcFile(String planId, String path) throws IOException, SQLException {
        BufferedWriter writer = null;
        boolean success = true;
        try {
            String[] params = new String[]{"C6_TEM_ISTRAN", "C6_DYE_ISTRAN", "C7_NTC",
                    "C16_NPBS", "C16_NPBW", "C16_NPBE", "C16_NPBN", "C16_NPFOR", "C16_NPFORT", "C16_NPSER", "C16_PDGINIT",
                    "C18_IPBS", "C18_JPBS", "C18_ISPBS", "C18_NPFORS", "C18_NPSERS",
                    "C22_NCSER2", "C22_NCSER3", "C23_NQSIJ", "C23_NQSER",
                    "C46_RKDYE"};
            Map<String, String> map = dao.getInputItemData(planId, params);
            List<Map<String, String>> data = dao.getXulieMapping(planId);
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("******************************************************************************\r\n" +
                    "*                                                                            *\r\n" +
                    "*  WELCOME TO THE ENVIRONMENTAL FLUID DYNAMICS COMPUTER CODE SERIES          *\r\n" +
                    "*  ORIGINALLY DEVELOPED BY JOHN M. HAMRICK.                                  *\r\n" +
                    "*                                                                            *\r\n" +
                    "*  THIS IS THE MASTER INPUT FILE EFDC.INP.                                   *\r\n" +
                    "*        FOR EFDC_DSI VERSION DATED AFTER FEB 2011                           *\r\n" +
                    "*        VERSION: 201105   VERSION ID CAN INFLUENCE INPUT FORMAT             *\r\n" +
                    "*                          DO NOT CHANGE WITHOUT CHECKING INPUT.FOR          *\r\n" +
                    "*                                                                            *\r\n" +
                    "*  GENERATED WITH DYNAMIC SOLUTIONS-INTERNATIONAL'S EFDC_Explorer7.1         *\r\n" +
                    "*                                                   Ver 130823               *\r\n" +
                    "******************************************************************************\r\n" +
                    "* PROJECT NAME: \r\n" +
                    "******************************************************************************\r\n" +
                    "C1 RUN TITLE\r\n" +
                    "*   TEXT DESCRIPTION UP TO 80 CHARACTERS IN LENGTH FOR THIS INPUT FILE AND RUN\r\n" +
                    "C1 TITLE \r\n" +
                    "Yuqiao Reservoir\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C1A  GRID CONFIGURATION AND TIME INTEGRATION MODE SELECTION\r\n" +
                    "*\r\n" +
                    "*  IGRIDH:        NOT USED\r\n" +
                    "*              \r\n" +
                    "*              \r\n" +
                    "*  INESTH:        NOT USED\r\n" +
                    "*                \r\n" +
                    "*                \r\n" +
                    "*  IGRIDV:        NOT USED\r\n" +
                    "*                 NOT USED\r\n" +
                    "*  IS2TIM:        0 THREE-TIME LEVEL INTEGRATION\r\n" +
                    "*                 1 TWO-TIME LEVEL INTEGRATION\r\n" +
                    "*  ISHOUSATONIC:  NOT USED\r\n" +
                    "*                 \r\n" +
                    "*              \r\n" +
                    "*\r\n" +
                    "C1A  IGRIDH  INESTH  IGRIDV  IS2TIM  ISHOUSATONIC\r\n" +
                    "          0       0       0       1       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C2 RESTART, GENERAL CONTROL AND AND DIAGNOSTIC SWITCHES\r\n" +
                    "*\r\n" +
                    "*  ISRESTI:  1 FOR READING INITIAL CONDITIONS FROM FILE restart.inp\r\n" +
                    "*           -1 AS ABOVE BUT ADJUST FOR CHANGING BOTTOM ELEVATION\r\n" +
                    "*            2 INITIALIZES A KC LAYER RUN FROM A KC/2 LAYER RUN FOR KC.GE.4\r\n" +
                    "*           10 FOR READING IC'S FROM restart.inp WRITTEN BEFORE 8 SEPT 92\r\n" +
                    "*  ISRESTO: -1 FOR WRITING RESTART FILE restart.out AT END OF RUN\r\n" +
                    "*            N INTEGER.GE.0 FOR WRITING restart*.out EVERY N REF TIME PERIODS\r\n" +
                    "*  ISRESTR:  1 FOR WRITING RESIDUAL TRANSPORT FILE RESTRAN.OUT\r\n" +
                    "*  ISGREGOR: 0/1 NOT/USING DATE STAMPED RESTART FILES\r\n" +
                    "*  ISLOG:    1 FOR WRITING LOG FILE EFDC.LOG\r\n" +
                    "*  IDUM:       NOT USED\r\n" +
                    "*            \r\n" +
                    "*            \r\n" +
                    "*  ISDIVEX:  1 FOR WRITING EXTERNAL MODE DIVERGENCE TO SCREEN\r\n" +
                    "*  ISNEGH:   1 FOR SEARCHING FOR NEGATIVE DEPTHS AND WRITING TO SCREEN\r\n" +
                    "*  ISMMC:    <0 FLAG TO GLOBALLY ACTIVATE WRITING EXTRA MODEL RESULTS LOG FILES\r\n" +
                    "*              \r\n" +
                    "*  ISBAL:    1 FOR ACTIVATING MASS, MOMENTUM AND ENERGY BALANCES AND\r\n" +
                    "*              WRITING RESULTS TO FILE bal.out\r\n" +
                    "*  IDUM:       NOT USED\r\n" +
                    "*  ISHOW:   >0 TO SHOW PUV&S ON SCREEN, SEE INSTRUCTIONS FOR FILE show.inp\r\n" +
                    "*\r\n" +
                    "C2  ISRESTI ISRESTO ISRESTR ISGREGOR  ISLOG ISDIVEX  ISNEGH   ISMMC   ISBAL    IDUM   ISHOW\r\n" +
                    "          0       0       0       0       0       0       2       0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C3 EXTERNAL MODE SOLUTION OPTION PARAMETERS AND SWITCHES\r\n" +
                    "*\r\n" +
                    "*  RP:        OVER RELAXATION PARAMETER\r\n" +
                    "*  RSQM:      TARGET SQUARE RESIDUAL OF ITERATIVE SOLUTION SCHEME\r\n" +
                    "*  ITERM:     MAXIMUN NUMBER OF ITERATIONS\r\n" +
                    "*  IRVEC:   0 CONJUGATE GRADIENT SOLUTION - NO SCALING\r\n" +
                    "*           9 CONJUGATE GRADIENT SOLUTION - SCALE BY MINIMUM DIAGONAL\r\n" +
                    "*          99 CONJUGATE GRADIENT SOLUTION - SCALE TO NORMAL FORM\r\n" +
                    "*             \r\n" +
                    "*        \r\n" +
                    "*  RPADJ:     RELAXATION PARAMETER FOR AUXILLARY POTENTIAL ADJUSTMENT\r\n" +
                    "*             OF THE MEAN MASS TRANSPORT ADVECTION FIELD\r\n" +
                    "*             (FOR RESEARCH PURPOSES)\r\n" +
                    "*  RSQMADJ:   TRAGET SQUARED RESIDUAL ERROR FOR ADJUSTMENT\r\n" +
                    "*             (FOR RESEARCH PURPOSES)\r\n" +
                    "*  ITRMADJ:   NUMBER OF INITIAL LOOPS TO HOLD TIMESTEP CONSTANT FOR DYN-STEP (DS-INTL)\r\n" +
                    "*  ITERHPM:   MAXIMUM ITERATIONS FOR STRONGLY NONLINER DRYING AND WETTING\r\n" +
                    "*             SCHEME (ISDRY=3 OR OR 4)  ITERHPM.LE.4\r\n" +
                    "*  IDRYCK:    ITERATIONS PER DRYING CHECK (ISDRY.GE.1)  2.LE.IDRYCK.LE.20\r\n" +
                    "*  ISDSOLV: 1 TO WRITE DIAGNOSTICS FILES FOR EXTERNAL MODE SOLVER\r\n" +
                    "*     FILT:   FILTER COEFFICIENT FOR 3 TIME LEVEL EXPLICIT ( 0.0625 )\r\n" +
                    "*\r\n" +
                    "C3       RP    RSQM   ITERM   IRVEC   RPADJ RSQMADJ NRAMPUP ITERHPM  IDRYCK ISDSOLV    FILT\r\n" +
                    "        1.8   1E-09     200       9     1.8   1E-16    1000       0      20       0   .0625\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C4 LONGTERM MASS TRANSPORT INTEGRATION ONLY SWITCHES\r\n" +
                    "*\r\n" +
                    "*  ISLTMT:   NOT USED\r\n" +
                    "*  ISSSMMT: 0 WRITES MEAN MASS TRANSPORT TO RESTRAN.OUT AFTER EACH\r\n" +
                    "*             AVERAGING PERIOD  (FOR WASP/ICM/RCA LINKAGE)\r\n" +
                    "*           1 WRITES MEAN MASS TRANSPORT TO RESTRAN.OUT AFTER LAST\r\n" +
                    "*             AVERAGING PERIOD  (FOR RESEARCH PURPOSES)\r\n" +
                    "*           2 DISABLES MEAN MASS TRANSPORT FIELD CALCULATIONS & RESTRAN.OUT\r\n" +
                    "*  ISLTMTS:  NOT USED\r\n" +
                    "*            \r\n" +
                    "*            \r\n" +
                    "*            \r\n" +
                    "*  ISIA:     NOT USED\r\n" +
                    "*            \r\n" +
                    "*  RPIA:     NOT USED\r\n" +
                    "*  RSQMIA:   NOT USED\r\n" +
                    "*  ITRMIA:   NOT USED\r\n" +
                    "*  ISAVEC:   NOT USED\r\n" +
                    "*\r\n" +
                    "C4   ISLTMT ISSSMMT ISLTMTS    ISIA    RPIA  RSQMIA  ITRMIA  ISAVEC\r\n" +
                    "          0       2       0       0     1.8   1E-10       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C5 MOMENTUM ADVEC AND HORIZ DIFF SWITCHES AND MISC SWITCHES\r\n" +
                    "*\r\n" +
                    "*  ISCDMA:  1 FOR CENTRAL DIFFERENCE MOMENTUM ADVECTION (USED FOR 3TL ONLY)\r\n" +
                    "*           0 FOR UPWIND DIFFERENCE MOMENTUM ADVECTION  (USED FOR 3TL ONLY)\r\n" +
                    "*           2 FOR EXPERIMENTAL UPWIND DIFF MOM ADV (FOR RESEARCH PURPOSES)\r\n" +
                    "*  ISAHMF:  1 TO ACTIVE HORIZONTAL MOMENTUM DIFFUSION\r\n" +
                    "*  ISDISP:  1 CALCULATE MEAN HORIZONTAL SHEAR DISPERSION TENSOR OVER LAST MEAN MASS TRANSPORT AVERAGING PERIOD\r\n" +
                    "*  ISWASP:  4 OR 5 TO WRITE FILES FOR WASP4 OR WASP5 MODEL LINKAGE, 17-WASP7HYDRO, 99 - CE-QUAL-ICM\r\n" +
                    "*  ISDRY:   0 NO WETTING & DRYING OF SHALLOW AREAS\r\n" +
                    "*           1 CONSTANT WETTING DEPTH SPECIFIED BY HWET ON CARD 11\r\n" +
                    "*             WITH NONLINEAR ITERATIONS SPECIFIED BY ITERHPM ON CARD C3\r\n" +
                    "*           2 VARIABLE WETTING DEPTH CALCULATED INTERNALLY IN CODE\r\n" +
                    "*             WITH NONLINEAR ITERATIONS SPECIFIED BY ITERHPM ON CARD C3\r\n" +
                    "*          11 SAME AS 1, WITHOUT NONLINEAR ITERATION  \r\n" +
                    "*         -11 SAME AS 11 BUT WITH CELL MASKING\r\n" +
                    "*          99 VARIABLE WETTING & DRYING USING CELL FACES\r\n" +
                    "*         -99 SAME AS 11 BUT WITH CELL MASKING\r\n" +
                    "*  ISQQ:    1 TO USE STANDARD TURBULENT INTENSITY ADVECTION SCHEME\r\n" +
                    "*           2 RESEARCH TURBULENT INTENSITY ADVECTION SCHEME\r\n" +
                    "*  ISRLID:  1 TO RUN IN RIGID LID MODE (NO FREE SURFACE)\r\n" +
                    "*  ISVEG:   1 TO IMPLEMENT VEGETATION RESISTANCE\r\n" +
                    "*           2 IMPLEMENT WITH DIAGNOSTICS TO FILE CBOT.LOG\r\n" +
                    "*  ISVEGL:  1 TO INCLUDE LAMINAR FLOW OPTION IN VEGETATION RESISTANCE\r\n" +
                    "*  ISITB:   1 FOR IMPLICIT BOTTOM & VEGETATION RESISTANCE IN EXTERNAL MODE\r\n" +
                    "*            \r\n" +
                    "*  ISEVER:  1 TO DEFAULT TO EVERGLADES HYDRO SOLUTION OPTIONS\r\n" +
                    "*  IINTPG:  0 ORIGINAL INTERNAL PRESSURE GRADIENT FORMULATION\r\n" +
                    "*           1 JACOBIAN FORMULATION\r\n" +
                    "*           2 FINITE VOLUME FORMULATION\r\n" +
                    "*  \r\n" +
                    "*\r\n" +
                    "C5   ISCDMA  ISAHMF  ISDISP  ISWASP   ISDRY    ISQQ  ISRLID   ISVEG  ISVEGL   ISITB  ISEVER  IINTPG\r\n" +
                    "          0       1       0       0     -99       1       0       0       0       2       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C6 DISSOLVED AND SUSPENDED CONSTITUENT TRANSPORT SWITCHES\r\n" +
                    "*  TURB INTENSITY=0,SAL=1,TEM=2,DYE=3,SFL=4,TOX=5,SED=6,SND=7,CWQ=8\r\n" +
                    "*\r\n" +
                    "*  ISTRAN:  1 OR GREATER TO ACTIVATE TRANSPORT\r\n" +
                    "*  ISTOPT:    NONZERO FOR TRANSPORT OPTIONS, SEE USERS MANUAL\r\n" +
                    "*  ISCDCA:  0 FOR STANDARD DONOR CELL UPWIND DIFFERENCE ADVECTION (3TL ONLY)\r\n" +
                    "*           1 FOR CENTRAL DIFFERENCE ADVECTION FOR THREE TIME LEVEL STEPS (3TL ONLY)\r\n" +
                    "*           2 FOR EXPERIMENTAL UPWIND DIFFERENCE ADVECTION (FOR RESEARCH) (3TL ONLY)\r\n" +
                    "*  ISADAC:  1 TO ACTIVATE ANTI-NUMERICAL DIFFUSION CORRECTION TO\r\n" +
                    "*             STANDARD DONOR CELL SCHEME\r\n" +
                    "*  ISFCT:   1 TO ADD FLUX LIMITING TO ANTI-NUMERICAL DIFFUSION CORRECTION\r\n" +
                    "*  ISPLIT:  1 TO OPERATOR SPLIT HORIZONTAL AND VERTICAL ADVECTION\r\n" +
                    "*             (FOR RESEARCH PURPOSES)\r\n" +
                    "*  ISADAH:  1 TO ACTIVATE ANTI-NUM DIFFUSION CORRECTION TO HORIZONTAL\r\n" +
                    "*             SPLIT ADVECTION STANDARD DONOR CELL SCHEME (FOR RESEARCH)\r\n" +
                    "*  ISADAV:  1 TO ACTIVATE ANTI-NUM DIFFUSION CORRECTION TO VERTICAL\r\n" +
                    "*             SPLIT ADVECTION STANDARD DONOR CELL SCHEME (FOR RESEARCH)\r\n" +
                    "*  ISCI:    1 TO READ CONCENTRATION FROM FILE restart.inp\r\n" +
                    "*  ISCO:    1 TO WRITE CONCENTRATION TO FILE restart.out\r\n" +
                    "*\r\n" +
                    "C6  ISTRAN   ISTOPT  ISCDCA  ISADAC   ISFCT  ISPLIT  ISADAH  ISADAV    ISCI    ISCO\r\n" +
                    "          0       1       0       0       0       0       0       0       1       1  !TURB 0\r\n" +
                    "          0       0       0       1       1       0       0       0       1       1  !SAL  1\r\n");
            writer.write("          " + map.get("C6_TEM_ISTRAN") + "       4       0       1       1       0       0       0       1       1  !TEM  2\r\n");
            writer.write("          " + map.get("C6_DYE_ISTRAN") + "       1       0       1       1       0       0       0       1       1  !DYE  3\r\n");
            writer.write("          0       0       0       1       1       0       0       0       1       0  !SFL  4\r\n" +
                    "          0       0       0       1       1       0       0       0       1       0  !TOX  5\r\n" +
                    "          0       0       0       1       1       0       0       0       1       0  !SED  6\r\n" +
                    "          0       0       0       1       1       0       0       0       1       0  !SND  7\r\n" +
                    "          0       0       0       1       1       0       0       0       1       0  !CWQ  8\r\n");
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C7 TIME-RELATED INTEGER PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  NTC:     NUMBER OF REFERENCE TIME PERIODS IN RUN\r\n" +
                    "*  NTSPTC:  NUMBER OF TIME STEPS PER REFERENCE TIME PERIOD\r\n" +
                    "*  NLTC:    NUMBER OF LINEARIZED REFERENCE TIME PERIODS\r\n" +
                    "*  NLTC:    NUMBER OF TRANSITION REF TIME PERIODS TO FULLY NONLINEAR\r\n" +
                    "*  NTCPP:   NUMBER OF REFERENCE TIME PERIODS BETWEEN FULL PRINTED OUTPUT\r\n" +
                    "*           TO FILE EFDC.OUT\r\n" +
                    "*  NTSTBC:  NUMBER OF TIME STEPS BETWEEN USING A TWO TIME LEVEL TRAPEZOIDAL\r\n" +
                    "*           CORRECTION TIME STEP, ** MASS BALANCE PRINT INTERVAL **\r\n" +
                    "*  NTCNB:   NUMBER OF REFERENCE TIME PERIODS WITH NO BUOYANCY FORCING (NOT USED)\r\n" +
                    "*  NTCVB:   NUMBER OF REF TIME PERIODS WITH VARIABLE BUOYANCY FORCING\r\n" +
                    "*  NTSMMT:  NUMBER OF NUMBER OF TIME STEPS TO AVERAGE OVER TO OBTAIN\r\n" +
                    "*           MASS BALANCE RESIDUALS OR MEAN MASS TRANSPORT VARIABLES (e.g. WASP Linkage)\r\n" +
                    "*  NFLTMT:  USE 1 (FOR RESEARCH PURPOSES)\r\n" +
                    "*  NDRYSTP: MIN NO. OF TIME STEPS A CELL REMAINS DRY AFTER INTIAL DRYING\r\n" +
                    "*           -NDRYSTP FOR ISDRY=-99 TO ACTIVATE WASTING WATER IN DRY CELLS \r\n" +
                    "C7      NTC NTSPTC     NLTC    NTTC   NTCPP  NTSTBC   NTCNB   NTCVB  NTSMMT  NFLTMT NDRYSTP\r\n");
            writer.write("         " + map.get("C7_NTC") + "    5760       0       0      10       0       0       0       0       1       0\r\n");
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C8 TIME-RELATED REAL PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  TCON:     CONVERSION MULTIPLIER TO CHANGE TBEGIN TO SECONDS\r\n" +
                    "*  TBEGIN:   TIME ORIGIN OF RUN\r\n" +
                    "*  TREF:     REFERENCE TIME PERIOD IN sec (i.e. 44714.16S OR 86400S)\r\n" +
                    "*  CORIOLIS: CONSTANT CORIOLIS PARAMETER IN 1/sec =2*7.29E-5*SIN(LAT)\r\n" +
                    "*  ISCORV:   1 TO READ VARIABLE CORIOLIS COEFFICIENT FROM LXLY.INP FILE\r\n" +
                    "*  ISCCA:    WRITE DIAGNOSTICS FOR MAX CORIOLIS-CURV ACCEL TO FILEEFDC.LOG\r\n" +
                    "*  ISCFL:    1 WRITE DIAGNOSTICS OF MAX THEORETICAL TIME STEP TO CFL.OUT\r\n" +
                    "*            GT 1  TIME STEP ONLY AT INTERVAL ISCFL FOR ENTIRE RUN\r\n" +
                    "*  ISCFLM:   1  TO MAP LOCATIONS OF MAX TIME STEPS OVER ENTIRE RUN\r\n" +
                    "*  DTSSFAC:  DYNAMIC TIME STEPPING IF 0.0.LT.DTSSFAC.LT.1.0\r\n" +
                    "*  DTSSDHDT: DYNAMIC TIME STEPPING RATE OF DEPTH CHANGE FACTOR (USED WHEN > 0)\r\n" +
                    "*\r\n" +
                    "C8       TCON    TBEGIN      TREF CORIOLIS     ISCORV    ISCCA     ISCFL    ISCFLM   DTSSFAC   DTSSDHDT\r\n" +
                    "        86400         0     86400 9.356E-05         0         1         1         1         0         0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C9 SPACE-RELATED AND SMOOTHING  PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  IC:      NUMBER OF CELLS IN I DIRECTION\r\n" +
                    "*  JC:      NUMBER OF CELLS IN J DIRECTION\r\n" +
                    "*  LC:      NUMBER OF ACTIVE CELLS IN HORIZONTAL + 2\r\n" +
                    "*  LVC:     NUMBER OF VARIABLE SIZE HORIZONTAL CELLS\r\n" +
                    "*  ISCO:    1 FOR CURVILINEAR-ORTHOGONAL GRID (LVC=LC-2)\r\n" +
                    "*  NDM:     NUMBER OF DOMAINS FOR HORIZONTAL DOMAIN DECOMPOSITION\r\n" +
                    "*           ( NDM=1, FOR MODEL EXECUTION ON A SINGLE PROCESSOR SYSTEM OR\r\n" +
                    "*             NDM=MM*NCPUS, WHERE MM IS AN INTEGER AND NCPUS IS THE NUMBER\r\n" +
                    "*             OF AVAILABLE CPU'S FOR MODEL EXECUTION ON A PARALLEL MULTIPLE PROCESSOR SYSTEM )\r\n" +
                    "*  LDM:     NUMBER OF WATER CELLS PER DOMAIN (LDM=(LC-2)/NDM, FOR MULTIPE VECTOR PROCESSORS, \r\n" +
                    "*             LDM MUST BE AN INTEGER MULTIPLE OF THE VECTOR LENGTH OR \r\n" +
                    "*             STRIDE NVEC THUS CONSTRAINING LC-2 TO BE AN INTEGER MULTIPLE OF NVEC )\r\n" +
                    "*  ISMASK:  1 FOR MASKING WATER CELL TO LAND OR ADDING THIN BARRIERS\r\n" +
                    "*            USING INFORMATION IN FILE MASK.INP\r\n" +
                    "*  ISPGNS:  1 FOR IMPLEMENTING A PERIODIC GRID IN COMP N-S DIRECTION OR\r\n" +
                    "*            CONNECTING ARBITRATY CELLS USING INFO IN FILE MAPPGNS.INP\r\n" +
                    "*  NSHMAX:  NUMBER OF DEPTH SMOOTHING PASSES\r\n" +
                    "*  NSBMAX:  NUMBER OF INITIAL SALINITY FIELD SMOOTHING PASSES\r\n" +
                    "*  WSMH:    DEPTH SMOOTHING WEIGHT\r\n" +
                    "*  WSMB:    SALINITY SMOOTHING WEIGHT\r\n" +
                    "* \r\n" +
                    "* \r\n" +
                    "* \r\n" +
                    "C9       IC      JC      LC     LVC    ISCO     NDM     LDM  ISMASK  ISPGNS  NSHMAX  NSBMAX    WSMH    WSMB\r\n" +
                    "         90      43    1928    1926       1       1    1926       0       0       0       0 0.03125 0.06250\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C9A  VERTICAL SPACE-RELATED PARAMETERS\r\n" +
                    "*      KC:      NUMBER OF VERTICAL LAYERS\r\n" +
                    "*\r\n" +
                    "*    KSIG:      NOT USED\r\n" +
                    "* ISETGVC:      NOT USED\r\n" +
                    "*             \r\n" +
                    "*             \r\n" +
                    "* SELVREF:     NOT USED\r\n" +
                    "* BELVREF:     NOT USED\r\n" +
                    "* ISGVCCK:     NOT USED\r\n" +
                    "*             \r\n" +
                    "*\r\n" +
                    "C9A  KC  KSIG  ISETGVC  SELVREF  BELVREF  ISGVCCK\r\n" +
                    "      1    1       1     0.000    -1.000       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C10 LAYER THICKNESS IN VERTICAL\r\n" +
                    "*\r\n" +
                    "*    K:  LAYER NUMBER, K=1,KC\r\n" +
                    "*  DZC:  DIMENSIONLESS LAYER THICKNESS (THICKNESSES MUST SUM TO 1.0)\r\n" +
                    "*\r\n" +
                    "*\r\n" +
                    "*\r\n" +
                    "C10  K   DZC \r\n" +
                    "     1 1.0000000\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C11 GRID, ROUGHNESS AND DEPTH PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  DX:       CARTESIAN CELL LENGTH IN X OR I DIRECTION\r\n" +
                    "*  DY:       CARTESION CELL LENGTH IN Y OR J DIRECTION\r\n" +
                    "*  DXYCVT:   MULTIPLY DX AND DY BY TO OBTAIN METERS\r\n" +
                    "*  IMD:      GREATER THAN 0 TO READ MODDXDY.INP FILE\r\n" +
                    "*  ZBRADJ:   LOG BDRY LAYER CONST OR VARIABLE ROUGH HEIGHT ADJ IN METERS\r\n" +
                    "*  ZBRCVRT:  LOG BDRY LAYER VARIABLE ROUGHNESS HEIGHT CONVERT TO METERS\r\n" +
                    "*  HMIN:     MINIMUM DEPTH OF INPUTS DEPTHS IN METERS\r\n" +
                    "*  HADJ:     ADJUCTMENT TO DEPTH FIELD IN METERS\r\n" +
                    "*  HCVRT:    CONVERTS INPUT DEPTH FIELD TO METERS\r\n" +
                    "*  HDRY:     DEPTH AT WHICH CELL OR FLOW FACE BECOMES DRY\r\n" +
                    "*  HWET:     DEPTH AT WHICH CELL OR FLOW FACE BECOMES WET\r\n" +
                    "*  BELADJ:   ADJUCTMENT TO BOTTOM BED ELEVATION FIELD IN METERS\r\n" +
                    "*  BELCVRT:  CONVERTS INPUT BOTTOM BED ELEVATION FIELD TO METERS\r\n" +
                    "*\r\n" +
                    "C11   DX      DY   DXYCVT    IMD  ZBRADJ ZBRCVRT    HMIN    HADJ   HCVRT    HDRY    HWET  BELADJ BELCVRT\r\n" +
                    "          1       1       1       0       0       1     .01       0       1      .1     .15       0       1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C11A TWO-LAYER MOMENTUM FLUX AND CURVATURE ACCELERATION CORRECTION FACTORS \r\n" +
                    "*  (ONLY USED FOR 2 TIME LEVEL SOLUTION & ISDRY=0  PMC-Check to see if still true)\r\n" +
                    "*  ICK2COR: 0 NO CORRECTION\r\n" +
                    "*  ICK2COR: 1 CORRECTION USING CK2UUC,CK2VVC,CK2UVC FOR CURVATURE\r\n" +
                    "*  ICK2COR: 2 CORRECTION USING CK2FCX,CK2FCY FOR CURVATURE\r\n" +
                    "*  CK2UUM:  CORRECTION FOR UU MOMENTUM FLUX\r\n" +
                    "*  CK2VVM:  CORRECTION FOR UU MOMENTUM FLUX\r\n" +
                    "*  CK2UVM:  CORRECTION FOR UU MOMENTUM FLUX\r\n" +
                    "*  CK2UUC:  CORRECTION FOR UU CURVATURE ACCELERATION   (NOT ACTIVE)\r\n" +
                    "*  CK2VVC:  CORRECTION FOR VV CURVATURE ACCELERATION   (NOT ACTIVE)\r\n" +
                    "*  CK2UVC:  CORRECTION FOR UV CURVATURE ACCELERATION   (NOT ACTIVE)\r\n" +
                    "*  CK2FCX:  CORRECTION FOR X EQUATION CURVATURE ACCELERATION \r\n" +
                    "*  CK2FCY:  CORRECTION FOR Y EQUATION CURVATURE ACCELERATION\r\n" +
                    "*\r\n" +
                    "C11A ICK2COR CK2UUM  CK2VVM  CK2UVM  CK2UUC  CK2VVC  CK2UVC  CK2FCX  CK2FCY\r\n" +
                    "          0   .0825   .0825   .0825   .0825   .0825   .0825   .0825   .0825\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C11B CORNER CELL BOTTOM STRESS CORRECTION OPTIONS (2TL ONLY)\r\n" +
                    "*\r\n" +
                    "*    ISCORTBC: 1 TO CORRECT BED STRESS AVERAGING TO CELL CENTERS IN CORNERS\r\n" +
                    "*              2 TO USE SPATIALLY VARYING CORRECTION FOR CELLS IN CORNERC.INP\r\n" +
                    "*   ISCORTBCD: 1 WRITE DIAGNOSTICS EVERY NSPTC TIME STEPS (NOT USED)\r\n" +
                    "*    FSCORTBC: CORRECTION FACTOR, 0.0 GE FSCORTBC LE 1.0\r\n" +
                    "*              1.0 = NO CORRECTION, 0.0 = MAXIMUM CORRECTION, 0.5 SUGGESTED\r\n" +
                    "*\r\n" +
                    "C11B ISCORTBC ISCORTBCD FSCORTBC\r\n" +
                    "          0       0      .5\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C12 TURBULENT DIFFUSION PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  AHO:      CONSTANT HORIZONTAL MOMENTUM AND MASS DIFFUSIVITY m*m/s\r\n" +
                    "*  AHD:      DIMESIONLESS HORIZONTAL MOMENTUM DIFFUSIVITY (ONLY FOR ISHDMF>0)\r\n" +
                    "*  AVO:      BACKGROUND, CONSTANT OR EDDY (KINEMATIC) VISCOSITY m*m/s\r\n" +
                    "*  ABO:      BACKGROUND, CONSTANT OR MOLECULAR DIFFUSIVITY m*m/s\r\n" +
                    "*  AVMX:     MAXIMUM KINEMATIC EDDY VISCOSITY m*m/s (DS-INTL)\r\n" +
                    "*  ABMX:     MAXIMUM EDDY DIFFUSIVITY m*m/s (DS-INTL)\r\n" +
                    "*  VISMUD:   CONSTANT FLUID MUD VISCOSITY m*m/s\r\n" +
                    "*  AVCON:    EQUALS ZERO FOR CONSTANT VERTICAL VISCOSITY AND DIFFUSIVITY\r\n" +
                    "*              WHICH ARE SET EQUAL TO AVO AND ABO, OTHERWISE SET TO 1.0\r\n" +
                    "*  ZBRWALL:  SIDE WALL LOG LAW ROUGHNESS HEIGHT. USED WHEN HORIZONTAL\r\n" +
                    "*              MOMENTUM DIFFUSION IS ACTIVE AND AHO OR AHD ARE NONZERO\r\n" +
                    "*\r\n" +
                    "C12     AHO       AHD       AVO       ABO      AVMX       ABMX   VISMUD     AVCON    ZBRWALL\r\n" +
                    "           40         0   .000001     1E-08   .000001     1E-08         0         1      .002\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C12A TURBULENCE CLOSURE OPTIONS\r\n" +
                    "*\r\n" +
                    "*      ISSTAB:   0 FOR GALPERIN et al. STABILITY FUNCTIONS IN CALAVBOLD (ISQQ=1)\r\n" +
                    "*                1 FOR GALPERIN et al. STABILITY FUNCTIONS              (ISQQ=1)\r\n" +
                    "*                2 FOR KANTHA AND CLAYSON (1994) STABILITY FUNCTIONS    (ISQQ=1)\r\n" +
                    "*                3 FOR KANTAH (2003) STABILITY FUNCTIONS                (ISQQ=1)\r\n" +
                    "*                  (NOTE: OPTION SELECTED HERE OVERRIDES ISTOPT(0) ON C6)\r\n" +
                    "*      ISSQL:    0 SETS QQ AND QQL STABILITY FUNCTIONS PROPORTIONAL TO\r\n" +
                    "*                  MOMENTUM STABILITY FUNCTIONS (EXCEPT FOR ISSTAB=3)\r\n" +
                    "*                1 SETS QQ AND QQL STABILITY FUNCTIONS TO CONSTANTS\r\n" +
                    "*                    (FOR ISSTAB = 0,1,2) THIS OPTION NOT ACTIVE\r\n" +
                    "*      ISAVBMN:  SET TO 1 TO ACTIVATE MIN VIS AND DIFF OF AVMN AND ABMN\r\n" +
                    "*      ISFAVB:   SET TO 1 OR 2 TO AVG OR SQRT FILTER AVV AND AVB\r\n" +
                    "*      ISINWV:   SET TO 2 TO WRITE EE_ARRAYS.OUT\r\n" +
                    "*      ISLLIM:   0 FOR NO LENGTH SCALE AND RIQMAX LIMITATIONS\r\n" +
                    "*                1 LIMIT RIQMAX IN STABILITY FUNCTION ONLY\r\n" +
                    "*                2 DIRECTLY LIMIT LENGTH SCALE AND LIMIT RIQMAX IN STABILITY FUNCTION\r\n" +
                    "*      IFPROX:   0 FOR NO WALL PROXIMITY FUNCTION\r\n" +
                    "*                1 FOR PARABOLIC OVER DEPTH WALL PROXIMITY FUNCTION\r\n" +
                    "*                2 FOR OPEN CHANNEL WALL PROXIMITY FUNCTION\r\n" +
                    "*      ISVTURB:  SET TO 1 TO INCLUDE VEGETATION GENERATED TURBULENCE PRODUCTION  (NOT USED)\r\n" +
                    "* BC_EDGEFACTOR: BOUNDARY CELLS MOMENTUM CORRECTION FACTOR (0 TO 1)\r\n" +
                    "*\r\n" +
                    "C12A ISSTAB   ISSQL ISAVBMN  ISFAVB  ISINWV  ISLLIM  IFPROX ISVTURB BC_EDGEFACTOR\r\n" +
                    "          1       0       0       2       1       2       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C13 TURBULENCE CLOSURE PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  VKC:    VON KARMAN CONSTANT\r\n" +
                    "*  CTURB1: TURBULENT CONSTANT (UNIVERSAL)\r\n" +
                    "*  CTURB2: TURBULENT CONSTANT (UNIVERSAL)\r\n" +
                    "*  CTE1:   TURBULENT CONSTANT (UNIVERSAL)\r\n" +
                    "*  CTE2:   TURBULENT CONSTANT (UNIVERSAL)\r\n" +
                    "*  CTE3:   TURBULENT CONSTANT (UNIVERSAL)\r\n" +
                    "*  CTE4:   TURBULENCE CONSTANT E4 (SOMETIMES CALL E3) WALL FUNCTION IN Q*Q*L EQUATION\r\n" +
                    "*  CTE5:   TURBULENCE CONSTANT E5 - 2ND OPEN CHANNEL WALL FUNCTION IN Q*Q*L EQUATION\r\n" +
                    "*  RIQMAX: MAXIMUM TURBULENT INTENSITY RICHARDSON NUMBER FOR STABLE CONDITIONS\r\n" +
                    "*  QQMIN:  MINIMUM TURBULENT INTENSITY SQUARED\r\n" +
                    "*  QQLMIN: MINIMUM TURBULENT INTENSITY SQUARED * LENGTH-SCALE\r\n" +
                    "*  DMLMIN: MINIMUM DIMENSIONLESS LENGTH SCALE\r\n" +
                    "*\r\n" +
                    "C13     VKC   CTURB1  CTURB2    CTE1    CTE2    CTE3    CTE4    CTE5  RIQMAX   QQMIN  QQLMIN  DMLMIN\r\n" +
                    "         .4    16.6    10.1     1.8       1     1.8    1.33     .25     .28   1E-08   1E-12   .0001\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C14 TIDAL & ATMOSPHERIC FORCING, GROUND WATER AND SUBGRID CHANNEL PARAMETERS\r\n" +
                    "*\r\n" +
                    "*   MTIDE:     NUMBER OF PERIOD (TIDAL) FORCING CONSTITUENTS\r\n" +
                    "*   NWSER:     NUMBER OF WIND TIME SERIES (0 SETS WIND TO ZERO)\r\n" +
                    "*   NASER:     NUMBER OF ATMOSPHERIC CONDITION TIME SERIES (0 SETS ALL  ZERO)\r\n" +
                    "*   ISGWI:     1 TO ACTIVATE SOIL MOISTURE BALANCE WITH DRYING AND WETTING\r\n" +
                    "*              2 TO ACTIVATE GROUNDWATER INTERACTION WITH BED AND WATER COL\r\n" +
                    "*  ISCHAN:    >0 ACTIVATE SUBGRID CHANNEL MODEL AND READ MODCHAN.INP\r\n" +
                    "*  ISWAVE:     1-FOR BOUNDARY LAYER IMPACTS ONLY (WAVEBL.INP),\r\n" +
                    "*              2-FOR BOUNDARY LAYER & CURRENT IMPACTS (WVnnn.INP)\r\n" +
                    "*              3-FOR INTERNALLY COMPUTED WIND WAVE BOUNDARY LAYER IMPACTS (DSI)\r\n" +
                    "*              4-FOR INTERNALLY COMPUTED WIND WAVE BOUNDARY LAYER AND CURRENT IMPACTS (DSI)\r\n" +
                    "* ITIDASM:     1 FOR TIDAL ELEVATION ASSIMILATION (NOT ACTIVE)\r\n" +
                    "*  ISPERC:     1 TO PERCOLATE OR ELIMINATE EXCESS WATER IN DRY CELLS\r\n" +
                    "* ISBODYF:     TO INCLUDE EXTERNAL MODE BODY FORCES FROM FBODY.INP\r\n" +
                    "*              1 FOR UNIFORM OVER DEPTH, 2 FOR SURFACE LAYER ONLY\r\n" +
                    "* ISPNHYDS: 1 FOR QUASI-NONHYDROSTATIC OPTION\r\n" +
                    "*\r\n" +
                    "C14   MTIDE   NWSER   NASER   ISGWI  ISCHAN  ISWAVE ITIDASM  ISPERC ISBODYF ISPNHYDS\r\n" +
                    "          0       1       1       0       0       0       0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C15 PERIODIC FORCING (TIDAL) CONSTITUENT SYMBOLS AND PERIODS\r\n" +
                    "*\r\n" +
                    "*  SYMBOL:  FORCING SYMBOL (CHARACTER VARIABLE) FOR TIDES, THE NOS SYMBOL\r\n" +
                    "*  PERIOD:  FORCING PERIOD IN SECONDS\r\n" +
                    "*\r\n" +
                    "C15 SYMBOL   PERIOD\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C16 SURFACE ELEVATION OR PRESSURE BOUNDARY CONDITION PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  NPBS:    NUMBER OF SURFACE ELEVATION OR PRESSURE BOUNDARY CONDITIONS\r\n" +
                    "*           CELLS ON SOUTH OPEN BOUNDARIES\r\n" +
                    "*  NPBW:    NUMBER OF SURFACE ELEVATION OR PRESSURE BOUNDARY CONDITIONS\r\n" +
                    "*           CELLS ON WEST OPEN BOUNDARIES\r\n" +
                    "*  NPBE:    NUMBER OF SURFACE ELEVATION OR PRESSURE BOUNDARY CONDITIONS\r\n" +
                    "*           CELLS ON EAST OPEN BOUNDARIES\r\n" +
                    "*  NPBN:    NUMBER OF SURFACE ELEVATION OR PRESSURE BOUNDARY CONDITIONS\r\n" +
                    "*           CELLS ON NORTH OPEN BOUNDARIES\r\n" +
                    "*  NPFOR:   NUMBER OF HARMONIC FORCINGS\r\n" +
                    "*  NPFORT:  FORCING TYPE, 0=CONSTANT, 1=LINEAR, 2= QUADRATIC VARIATION\r\n" +
                    "*  NPSER:   NUMBER OF TIME SERIES FORCINGS\r\n" +
                    "*  PDGINIT: ADD THIS CONSTANT ADJUSTMENT GLOBALLY TO THE SURFACE ELEVATION\r\n" +
                    "*\r\n" +
                    "C16    NPBS    NPBW    NPBE    NPBN   NPFOR  NPFORT   NPSER PDGINIT\r\n");
            writer.write("          " + map.get("C16_NPBS") + "       " + map.get("C16_NPBW") + "       " + map.get("C16_NPBE") + "       " + map.get("C16_NPBN") + "       " + map.get("C16_NPFOR") + "       " + map.get("C16_NPFORT") + "       " + map.get("C16_NPSER") + "       " + map.get("C16_PDGINIT") + "\r\n");
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C17 PERIODIC FORCING (TIDAL) SURF ELEV OR PRESSURE BOUNDARY COND. FORCINGS\r\n" +
                    "*\r\n" +
                    "*  NPFOR:     FORCING NUMBER\r\n" +
                    "*  SYMBOL:    FORCING SYMBOL (FOR REFERENCE HERE ONLY)\r\n" +
                    "*  AMPLITUDE:  AMPLITUDE IN M (PRESSURE DIVIDED BY RHO*G), NPFORT=0\r\n" +
                    "*               COSINE AMPLITUDE IN M, NPFORT.GE.1\r\n" +
                    "*  PHASE:      FORCING PHASE RELATIVE TO TBEGIN IN SECONDS, NPFORT=0\r\n" +
                    "*               SINE AMPLITUDE IN M, NPFORT.GE.1\r\n" +
                    "*  NOTE:  FOR NPFORT=0 SINGLE AMPLITUDE AND PHASE ARE READ, FOR NPFORT=1 \r\n" +
                    "*         CONST AND LINEAR COS AND SIN AMPS ARE READ FOR EACH FORCING, FOR  \r\n" +
                    "*         NPFORT=2, CONST, LINEAR, QUAD COS AND SIN AMPS ARE READ FOR EACH \r\n" +
                    "*         FOR EACH FORCING \r\n" +
                    "*\r\n" +
                    "C17   NPFOR  SYMBOL AMPLITUDE PHASE\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C18 PERIODIC FORCING (TIDAL) SURF ELEV OR PRESSURE ON SOUTH OPEN BOUNDARIES\r\n" +
                    "*  IPBS:     I CELL INDEX OF BOUNDARY CELL\r\n" +
                    "*  JPBS:     J CELL INDEX OF BOUNDARY CELL\r\n" +
                    "*  ISPBS: 0 FOR ELEVATION SPECIFIED\r\n" +
                    "*         1 FOR RADIATION-SEPARATION CONDITION, ZERO TANGENTIAL VELOCITY\r\n" +
                    "*         2 FOR RADIATION-SEPARATION CONDITION, FREE TANGENTIAL VELOCITY\r\n" +
                    "*  NPFORS:   APPLY HARMONIC FORCING NUMBER NPFORS\r\n" +
                    "*  NPSERS:   APPLY TIME SERIES FORCING NUMBER NPSERS\r\n" +
                    "*  NPSERS1:  APPLY TIME SERIES FORCING NUMBER NPSERS1 FOR 2ND SERIES (NPFORT.GE.1)\r\n" +
                    "*  TPCOORDS: TANGENTIAL COORDINATE ALONG BOUNDARY                    (NPFORT.GE.1)\r\n" +
                    "*\r\n" +
                    "C18    IPBS    JPBS   ISPBS  NPFORS  NPSERS\r\n");
            //start todo C18 ÔÝÊ±Ã»Öµ
//            writer.write("");
            // end
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C19 PERIODIC FORCING (TIDAL) SURF ELEV OR PRESSURE ON WEST OPEN BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  IPBW: SEE CARD 18\r\n" +
                    "*  JPBW:\r\n" +
                    "*  ISPBW:\r\n" +
                    "*  NPFORW:\r\n" +
                    "*  NPSERW:\r\n" +
                    "*  TPCOORDW: \r\n" +
                    "*\r\n" +
                    "C19    IPBW    JPBW   ISPBW  NPFORW  NPSERW\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C20 PERIODIC FORCING (TIDAL) SURF ELEV OR PRESSURE ON EAST OPEN BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  IPBE: SEE CARD 18\r\n" +
                    "*  JPBE:\r\n" +
                    "*  ISPBE:\r\n" +
                    "*  NPFORE:\r\n" +
                    "*  NPSERE:\r\n" +
                    "*  TPCOORDE: \r\n" +
                    "*\r\n" +
                    "C20    IPBE    JPBE   ISPBE  NPFORE  NPSERE\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C21 PERIODIC FORCING (TIDAL) SURF ELEV OR PRESSURE ON NORTH OPEN BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  IPBN: SEE CARD 18\r\n" +
                    "*  JPBN:\r\n" +
                    "*  ISPBN:\r\n" +
                    "*  NPFORN:\r\n" +
                    "*  NPSERN:\r\n" +
                    "*  TPCOORDN: \r\n" +
                    "*\r\n" +
                    "C21    IPBN    JPBN   ISPBN  NPFORN  NPSERN\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C22 SPECIFY NUM OF SEDIMENT AND TOXICS AND NUM OF CONCENTRATION TIME SERIES\r\n" +
                    "*\r\n" +
                    "*  NTOX:   NUMBER OF TOXIC CONTAMINANTS (DEFAULT = 1)\r\n" +
                    "*  NSED:   NUMBER OF COHESIVE SEDIMENT SIZE CLASSES (DEFAULT = 1)\r\n" +
                    "*  NSND:   NUMBER OF NON-COHESIVE SEDIMENT SIZE CLASSES (DEFAULT = 1)\r\n" +
                    "*  NCSER1: NUMBER OF SALINITY TIME SERIES\r\n" +
                    "*  NCSER2: NUMBER OF TEMPERATURE TIME SERIES\r\n" +
                    "*  NCSER3: NUMBER OF DYE CONCENTRATION TIME SERIES\r\n" +
                    "*  NCSER4: NUMBER OF SHELLFISH LARVAE CONCENTRATION TIME SERIES\r\n" +
                    "*  NCSER5: NUMBER OF TOXIC CONTAMINANT CONCENTRATION TIME SERIES\r\n" +
                    "*          EACH TIME SERIES MUST HAVE DATA FOR NTOX TOXICICANTS\r\n" +
                    "*  NCSER6: NUMBER OF COHESIVE SEDIMENT CONCENTRATION TIME SERIES\r\n" +
                    "*          EACH TIME SERIES MUST HAVE DATA FOR NSED COHESIVE SEDIMENTS\r\n" +
                    "*  NCSER7: NUMBER OF NON-COHESIVE SEDIMENT CONCENTRATION TIME SERIES\r\n" +
                    "*          EACH TIME SERIES MUST HAVE DATA FOR NSND NON-COHESIVE SEDIMENTS\r\n" +
                    "*  ISSBAL: SET TO 1 FOR SEDIENT MASS BALANCE           ! JOHN & JI, 4/25/97\r\n" +
                    "*\r\n" +
                    "C22    NTOX    NSED    NSND  NCSER1  NCSER2  NCSER3  NCSER4  NCSER5  NCSER6  NCSER7  ISSBAL\r\n");
                String C22_NCSER2 = map.get("C22_NCSER2");
                if(StringUtils.isEmpty(map.get("C22_NCSER2"))){
                        C22_NCSER2="1";
                }
                String C22_NCSER3 = map.get("C22_NCSER3");
                if(StringUtils.isEmpty(map.get("C22_NCSER3"))){
                        C22_NCSER3="1";
                }
            writer.write("          0       0       0       0       " + C22_NCSER2 + "       " + C22_NCSER3 + "       0       0       0       0       0\r\n");
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C23 VELOCITY, VOLUMN SOURCE/SINK, FLOW CONTROL, AND WITHDRAWAL/RETURN DATA\r\n" +
                    "*\r\n" +
                    "*  NQSIJ:  NUMBER OF CONSTANT AND/OR TIME SERIES SPECIFIED SOURCE/SINK\r\n" +
                    "*          LOCATIONS (RIVER INFLOWS,ETC)             .\r\n" +
                    "*  NQJPIJ: NUMBER OF CONSTANT AND/OR TIME SERIES SPECIFIED SOURCE\r\n" +
                    "*          LOCATIONS TREATED AS JETS/PLUMES          .\r\n" +
                    "*  NQSER:  NUMBER OF VOLUME SOURCE/SINK TIME SERIES\r\n" +
                    "*  NQCTL:  NUMBER OF PRESSURE CONTROLED WITHDRAWAL/RETURN PAIRS\r\n" +
                    "*  NQCTLT: NUMBER OF PRESSURE CONTROLED WITHDRAWAL/RETURN TABLES\r\n" +
                    "*  NQWR:   NUMBER OF CONSTANT OR TIME SERIES SPECIFIED WITHDRAWL/RETURN\r\n" +
                    "*          PAIRS\r\n" +
                    "*  NQWRSR: NUMBER OF TIME SERIES SPECIFYING WITHDRAWL,RETURN AND\r\n" +
                    "*          CONCENTRATION RISE SERIES\r\n" +
                    "*  ISDIQ:  SET TO 1 TO WRITE DIAGNOSTIC FILE, DIAQ.OUT\r\n" +
                    "*\r\n" +
                    "C23   NQSIJ  NQJPIJ   NQSER   NQCTL  NQCTLT    NQWR  NQWRSR   ISDIQ\r\n");
                String C23_NQSIJ = map.get("C23_NQSIJ");
                if(StringUtils.isEmpty(map.get("C23_NQSIJ"))){
                        C23_NQSIJ="1";
                }
                String C23_NQSER = map.get("C23_NQSER");
                if(StringUtils.isEmpty(map.get("C23_NQSER"))){
                        C23_NQSER="1";
                }
            writer.write("       " + C23_NQSIJ + "       0       " +C23_NQSER + "       0       0       0       0       0\r\n");
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C24 VOLUMETRIC SOURCE/SINK LOCATIONS, MAGNITUDES, AND CONCENTRATION SERIES\r\n" +
                    "*\r\n" +
                    "*  IQS:      I CELL INDEX OF VOLUME SOURCE/SINK\r\n" +
                    "*  JQS:      J CELL INDEX OF VOLUME SOURCE/SINK\r\n" +
                    "*  QSSE:     CONSTANT INFLOW/OUTFLOW RATE IN M*m*m/s\r\n" +
                    "*  NQSMUL:   MULTIPLIER SWITCH FOR CONSTANT AND TIME SERIES VOL S/S\r\n" +
                    "*            = 0  MULT BY 1. FOR NORMAL IN/OUTFLOW (L*L*L/T)\r\n" +
                    "*            = 1  MULT BY DY FOR LATERAL IN/OUTFLOW (L*L/T) ON U FACE\r\n" +
                    "*            = 2  MULT BY DX FOR LATERAL IN/OUTFLOW (L*L/T) ON V FACE\r\n" +
                    "*            = 3  MULT BY DX+DY FOR LATERAL IN/OUTFLOW (L*L/T) ON U&V FACES\r\n" +
                    "*  NQSMFF:   IF NON ZERO ACCOUNT FOR VOL S/S MOMENTUM FLUX\r\n" +
                    "*            = 1  MOMENTUM FLUX ON NEG U FACE\r\n" +
                    "*            = 2  MOMENTUM FLUX ON NEG V FACE\r\n" +
                    "*            = 3  MOMENTUM FLUX ON POS U FACE\r\n" +
                    "*            = 4  MOMENTUM FLUX ON POS V FACE\r\n" +
                    "*  IQSERQ:   ID NUMBER OF ASSOCIATED VOLUMN FLOW TIME SERIES\r\n" +
                    "*  ICSER1:   ID NUMBER OF ASSOCIATED SALINITY TIME SERIES\r\n" +
                    "*  ICSER2:   ID NUMBER OF ASSOCIATED TEMPERATURE TIME SERIES\r\n" +
                    "*  ICSER3:   ID NUMBER OF ASSOCIATED DYE CONC TIME SERIES\r\n" +
                    "*  ICSER4:   ID NUMBER OF ASSOCIATED SHELL FISH LARVAE RELEASE TIME SERIES\r\n" +
                    "*  ICSER5:   ID NUMBER OF ASSOCIATED TOXIC CONTAMINANT CONC TIME SERIES\r\n" +
                    "*  ICSER6:   ID NUMBER OF ASSOCIATED COHESIVE SEDIMENT CONC TIME SERIES\r\n" +
                    "*  ICSER7:   ID NUMBER OF ASSOCIATED NON-COHESIVE SED CONC TIME SERIES\r\n" +
                    "*  QSFACTOR: FRACTION OF TIME SERIES FLOW NQSERQ ASSIGNED TO THIS CELL\r\n" +
                    "*  \r\n" +
                    "C24  IQS     JQS     QSSE     NQSMUL  NQSMFF  IQSERQ  ICSER1  ICSER2  ICSER3  ICSER4  ICSER5  ICSER6  ICSER7   QSFACTOR  ! ID\r\n");
            for (int i = 1; i < data.size(); i++) {
                writer.write("       " + data.get(i).get("i_code") + "      " + data.get(i).get("j_code") + "  0.0000E+00       0       0       " + data.get(i).get("IQSERQ") + "       0       " + data.get(i).get("ICSER2") + "       " + data.get(i).get("ICSER3") + "       0       0       0       0  1.0000E+00\r\n");
            }
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C25 TIME CONSTANT INFLOW CONCENTRATIONS FOR TIME CONSTANT VOLUMETRIC SOURCES\r\n" +
                    "*\r\n" +
                    "*  SAL: SALT CONCENTRATION CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  TEM: TEMPERATURE CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  DYE: DYE CONCENTRATION CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  SFL: SHELL FISH LARVAE CONCENTRATION CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  TOX: NTOX TOXIC CONTAMINANT CONCENTRATIONS CORRESPONDING TO\r\n" +
                    "*       INFLOW ABOVE  WRITTEN AS TOXC(N), N=1,NTOX A SINGLE DEFAULT\r\n" +
                    "*       VALUE IS REQUIRED EVEN IF TOXIC TRANSPORT IS NOT ACTIVE\r\n" +
                    "*\r\n" +
                    "C25    SAL       TEM       DYE       SFL  ! ID\r\n" +
                    "         0         0         0         0  ! ShaLi_Flow in boundary\r\n" +
                    "         0         0         0         0  ! Lin_Flow in boundary\r\n" +
                    "         0         0         0         0  ! Flow out boundary\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C26 TIME CONSTANT INFLOW CONCENTRATIONS FOR TIME CONSTANT VOLUMETRIC SOURCES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED COHESIVE SEDIMENT CONCENTRATIONS CORRESPONDING TO\r\n" +
                    "*       INFLOW ABOVE  WRITTEN AS SEDC(N), N=1,NSED. I.E., THE FIRST\r\n" +
                    "*       NSED VALUES ARE COHESIVE A SINGLE DEFAULT VALUE IS REQUIRED\r\n" +
                    "*       EVEN IF COHESIVE SEDIMENT TRANSPORT IS INACTIVE\r\n" +
                    "*  SND: NSND NON-COHESIVE SEDIMENT CONCENTRATIONS CORRESPONDING TO\r\n" +
                    "*       INFLOW ABOVE  WRITTEN AS SND(N), N=1,NSND. I.E., THE LAST\r\n" +
                    "*       NSND VALUES ARE NON-COHESIVE. A SINGLE DEFAULT VALUE IS\r\n" +
                    "*       REQUIRED EVEN IF NON-COHESIVE SEDIMENT TRANSPORT IS INACTIVE\r\n" +
                    "*\r\n" +
                    "C26   SED1    SND1\r\n" +
                    "  ! ShaLi_Flow in boundary\r\n" +
                    "  ! Lin_Flow in boundary\r\n" +
                    "  ! Flow out boundary\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C27 JET/PLUME SOURCE LOCATIONS, GEOMETRY AND ENTRAINMENT PARAMETERS\r\n" +
                    "*\r\n" +
                    "*    ID: ID COUNTER FOR JET/PLUME\r\n" +
                    "*  ICAL: 0 BYPASS,  1 ACTIVE (NORMAL - TOTAL LAYER FLOW AT DIFFUSER),  2 - W/R (USE W/R SERIES)\r\n" +
                    "*  IQJP: I CELL INDEX OF JET/PLUME\r\n" +
                    "*  JQJP: J CELL INDEX OF JET/PLUME\r\n" +
                    "*  KQJP: K CELL INDEX OF JET/PLUME (DEFAULT, QJET=0 OR JET COMP DIVERGES)\r\n" +
                    "* NPORT: NUMBER OF IDENTIAL PORTS IN THIS CELL\r\n" +
                    "*  XJET: LOCAL EAST JET LOCATION RELATIVE TO DISCHARGE CELL CENTER (m) (NOT USED)\r\n" +
                    "*  YJET: LOCAL NORTH JET LOCATION RELATIVE TO DISCHARGE CELL CENTER (m)(NOT USED)\r\n" +
                    "*  ZJET: ELEVATION OF DISCHARGE (m)\r\n" +
                    "* PHJET: VERTICAL JET ANGLE POSITIVE FROM HORIZONTAL (DEGREES)\r\n" +
                    "* THJET: HORIZONTAL JET ANGLE POS COUNTER CLOCKWISE FROM EAST (DEGREES)\r\n" +
                    "*  DJET: DIAMETER OF DISCHARGE PORT (m)\r\n" +
                    "*  CFRD: ADJUSTMENT FACTOR FOR FROUDE NUMBER\r\n" +
                    "* DJPER: ENTRAINMENT ERROR CRITERIA\r\n" +
                    "*\r\n" +
                    "C27      ID    ICAL    IQJP    JQJP    KQJP   NPORT    XJET    YJET    ZJET   PHJET   THJET    DJET    CFRD   DJPER\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C28 JET/PLUME SOLUTION CONTROL AND OUTPUT CONTROL PARAMETERS\r\n" +
                    "*\r\n" +
                    "*     ID:  ID COUNTER FOR JET/PLUME\r\n" +
                    "*   NJEL:  MAXIMUM NUMBER OF ELEMENTS ALONG JET/PLUME LENGTH\r\n" +
                    "*  NJPMX:  MAXIMUM NUMBER OF ITERATIONS\r\n" +
                    "*  ISENT:  0 USE MAXIMUM OF SHEAR AND FORCED ENTRAINMENT\r\n" +
                    "*          1 USE SUM OF SHEAR AND FORCED ENTRAINMENT\r\n" +
                    "*  ISTJP:  0 STOP AT SPECIFIED NUMBER OF ELEMENTS\r\n" +
                    "*          1 STOP WHEN CENTERLINE PENETRATES BOTTOM OR SURFACE\r\n" +
                    "*          2 STOP WITH BOUNDARY PENETRATES BOTTOM OR SURFACE\r\n" +
                    "*  NUDJP: FREQUENCY FOR UPDATING JET/PLUME (NUMBER OF TIME STEPS)\r\n" +
                    "*   IOJP: 1 FOR FULL ASCII, 2 FOR COMPACT ASCII OUTPUT AT EACH UPDATE\r\n" +
                    "*         3 FOR FULL AND COMPACT ASCII OUTPUT, 4 FOR BINARY OUTPUT\r\n" +
                    "*   IPJP: NUMBER OF SPATIAL PRINT/SAVE POINT IN VERTICAL\r\n" +
                    "*  ISDJP: 1 WRITE DIAGNOSTIS TO JPLOG__.OUT\r\n" +
                    "*  IUPJP:   I INDEX OF UPSTREAM WITHDRAWAL CELL IF ICAL=2 \r\n" +
                    "*  JUPJP:   J INDEX OF UPSTREAM WITHDRAWAL CELL IF ICAL=2 \r\n" +
                    "*  KUPJP:   K INDEX OF UPSTREAM WITHDRAWAL CELL IF ICAL=2 \r\n" +
                    "*\r\n" +
                    "C28      ID    NJEL   NJPMX   ISENT   ISTJP   NUDJP    IOJP    IPJP   ISDJP   IUPJP   JUPJP   KUPJP\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C29 JET/PLUME SOURCE PARAMETERS AND DISCHARGE/CONCENTRATION SERIES IDS\r\n" +
                    "*\r\n" +
                    "*        ID: ID COUNTER FOR JET/PLUME\r\n" +
                    "*      QQJP: CONSTANT JET/PLUME FLOW RATE IN M*m*m/s\r\n" +
                    "*              FOR ICAL = 1 OR 2  (FOR SINGLE PORT)\r\n" +
                    "*   NQSERJP: ID NUMBER OF ASSOCIATED VOLUMN FLOW TIME SERIES\r\n" +
                    "* NQWRSERJP: ID NUMBER OF ASSOCIATED WITHDAWAL-RETURN TIME SERIES (ICAL=2)\r\n" +
                    "*    ICSER1: ID NUMBER OF ASSOCIATED SALINITY TIME SERIES\r\n" +
                    "*    ICSER2: ID NUMBER OF ASSOCIATED TEMPERATURE TIME SERIES\r\n" +
                    "*    ICSER3: ID NUMBER OF ASSOCIATED DYE CONC TIME SERIES\r\n" +
                    "*    ICSER4: ID NUMBER OF ASSOCIATED SHELL FISH LARVAE RELEASE TIME SERIES\r\n" +
                    "*    ICSER5: ID NUMBER OF ASSOCIATED TOXIC CONTAMINANT CONC TIME SERIES\r\n" +
                    "*    ICSER6: ID NUMBER OF ASSOCIATED COHESIVE SEDIMENT CONC TIME SERIES\r\n" +
                    "*    ICSER7: ID NUMBER OF ASSOCIATED NON-COHESIVE SED CONC TIME SERIES\r\n" +
                    "*\r\n" +
                    "C29      ID    QQJP NQSERJP NQWRSERJP ICSER1 ICSER2  ICSER3  ICSER4  ICSER5  ICSER6  ICSER7\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C30 TIME CONSTANT INFLOW CONCENTRATIONS FOR TIME CONSTANT JET/PLUME SOURCES\r\n" +
                    "*\r\n" +
                    "*  SAL: SALT CONCENTRATION CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  TEM: TEMPERATURE CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  DYE: DYE CONCENTRATION CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  SFL: SHELL FISH LARVAE CONCENTRATION CORRESPONDING TO INFLOW ABOVE\r\n" +
                    "*  TOX: NTOX TOXIC CONTAMINANT CONCENTRATIONS CORRESPONDING TO\r\n" +
                    "*       INFLOW ABOVE  WRITTEN AS TOXC(N), N=1,NTOX A SINGLE DEFAULT\r\n" +
                    "*       VALUE IS REQUIRED EVEN IF TOXIC TRANSPORT IS NOT ACTIVE\r\n" +
                    "*\r\n" +
                    "C30    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C31 TIME CONSTANT INFLOW CONCENTRATIONS FOR TIME CONSTANT JET/PLUME SOURCES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED COHESIVE SEDIMENT CONCENTRATIONS CORRESPONDING TO\r\n" +
                    "*       INFLOW ABOVE  WRITTEN AS SEDC(N), N=1,NSED. I.E., THE FIRST\r\n" +
                    "*       NSED VALUES ARE COHESIVE A SINGLE DEFAULT VALUE IS REQUIRED\r\n" +
                    "*       EVEN IF COHESIVE SEDIMENT TRANSPORT IS INACTIVE\r\n" +
                    "*  SND: NSND NON-COHESIVE SEDIMENT CONCENTRATIONS CORRESPONDING TO\r\n" +
                    "*       INFLOW ABOVE  WRITTEN AS SND(N), N=1,NSND. I.E., THE LAST\r\n" +
                    "*       NSND VALUES ARE NON-COHESIVE. A SINGLE DEFAULT VALUE IS\r\n" +
                    "*       REQUIRED EVEN IF NON-COHESIVE SEDIMENT TRANSPORT IS INACTIVE\r\n" +
                    "*\r\n" +
                    "C31   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C32 SURFACE ELEV OR PRESSURE DEPENDENT FLOW INFORMATION\r\n" +
                    "*\r\n" +
                    "*  IQCTLU: I INDEX OF UPSTREAM OR WITHDRAWAL CELL\r\n" +
                    "*  JQCTLU: J INDEX OF UPSTREAM OR WITHDRAWAL CELL\r\n" +
                    "*  IQCTLD: I INDEX OF DOWNSTREAM OR RETURN CELL\r\n" +
                    "*  JQCTLD: J INDEX OF DOWNSTREAM OR RETURN CELL\r\n" +
                    "*  NQCTYP: FLOW CONTROL TYPE\r\n" +
                    "*          = -1 RATING CURVED FLOW AS FUNCTION UPSTREAM DEPTH\r\n" +
                    "*          = 0  HYDRAULIC STRUCTURE: INSTANT FLOW DRIVEN BY ELEVATION\r\n" +
                    "*               OR PRESSURE DIFFERCENCE TABLE\r\n" +
                    "*          = 1  ACCELERATING FLOW THROUGH TIDAL INLET\r\n" +
                    "*  NQCTLQ: ID NUMBER OF CONTROL CHARACTERIZATION TABLE\r\n" +
                    "*  NQCMUL: MULTIPLIER SWITCH FOR FLOWS FROM UPSTREAM CELL\r\n" +
                    "*          = 0  MULT BY 1. FOR CONTROL TABLE IN (L*L*L/T)\r\n" +
                    "*          = 1  MULT BY DY FOR CONTROL TABLE IN (L*L/T) ON U FACE\r\n" +
                    "*          = 2  MULT BY DX FOR CONTROL TABLE IN (L*L/T) ON V FACE\r\n" +
                    "*          = 3  MULT BY DX+DY FOR CONTROL TABLE IN (L*L/T) ON U&V FACES\r\n" +
                    "*  NQCMFU: IF NON ZERO ACCOUNT FOR FLOW MOMENTUM FLUX IN UPSTREAM CELL\r\n" +
                    "*          = 1  MOMENTUM FLUX ON NEG U FACE\r\n" +
                    "*          = 2  MOMENTUM FLUX ON NEG V FACE\r\n" +
                    "*          = 3  MOMENTUM FLUX ON POS U FACE\r\n" +
                    "*          = 4  MOMENTUM FLUX ON POS V FACE\r\n" +
                    "*  NQCMFD: IF NON ZERO ACCOUNT FOR FLOW MOMENTUM FLUX IN DOWNSTREAM CELL\r\n" +
                    "*          = 1  MOMENTUM FLUX ON NEG U FACE\r\n" +
                    "*          = 2  MOMENTUM FLUX ON NEG V FACE\r\n" +
                    "*          = 3  MOMENTUM FLUX ON POS U FACE\r\n" +
                    "*          = 4  MOMENTUM FLUX ON POS V FACE\r\n" +
                    "*  BQCMFU: UPSTREAM MOMENTUM FLUX WIDTH (m)\r\n" +
                    "*  BQCMFD: DOWNSTREAM MOMENTUM FLUX WIDTH (m)\r\n" +
                    "*\r\n" +
                    "C32  IQCTLU  JQCTLU  IQCTLD  JQCTLD  NQCTYP  NQCTLQ  NQCMUL   NQC_U   NQC_D   BQC_U   BQC_D\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C33 FLOW WITHDRAWAL, HEAT OR MATERIAL ADDITION, AND RETURN DATA\r\n" +
                    "*\r\n" +
                    "*  IWRU:     I INDEX OF UPSTREAM OR WITHDRAWAL CELL\r\n" +
                    "*  JWRU:     J INDEX OF UPSTREAM OR WITHDRAWAL CELL\r\n" +
                    "*  KWRU:     K INDEX OF UPSTREAM OR WITHDRAWAL LAYER\r\n" +
                    "*  IWRD:     I INDEX OF DOWNSTREAM OR RETURN CELL\r\n" +
                    "*  JWRD:     J INDEX OF DOWNSTREAM OR RETURN CELL\r\n" +
                    "*  KWRD:     J INDEX OF DOWNSTREAM OR RETURN LAYER\r\n" +
                    "*  QWRE:       CONSTANT VOLUME FLOW RATE FROM WITHDRAWAL TO RETURN\r\n" +
                    "*  NQWRSERQ: ID NUMBER OF ASSOCIATED VOLUMN WITHDRAWAL-RETURN FLOW AND\r\n" +
                    "*               CONCENTRATION RISE TIME SERIES\r\n" +
                    "*  NQWRMFU:  IF NON ZERO ACCOUNT FOR WITHDRAWAL FLOW MOMENTUM FLUX\r\n" +
                    "*            = 1  MOMENTUM FLUX ON NEG U FACE\r\n" +
                    "*            = 2  MOMENTUM FLUX ON NEG V FACE\r\n" +
                    "*            = 3  MOMENTUM FLUX ON POS U FACE\r\n" +
                    "*            = 4  MOMENTUM FLUX ON POS V FACE\r\n" +
                    "*  NQWRMFD:   IF NON ZERO ACCOUNT FOR RETURN FLOW MOMENTUM FLUX\r\n" +
                    "*            = 1  MOMENTUM FLUX ON NEG U FACE\r\n" +
                    "*            = 2  MOMENTUM FLUX ON NEG V FACE\r\n" +
                    "*            = 3  MOMENTUM FLUX ON POS U FACE\r\n" +
                    "*            = 4  MOMENTUM FLUX ON POS V FACE\r\n" +
                    "*  BQWRMFU:  UPSTREAM MOMENTUM FLUX WIDTH (m)\r\n" +
                    "*  BQWRMFD:  DOWNSTREAM MOMENTUM FLUX WIDTH (m)\r\n" +
                    "*  ANGWRMFD: ANGLE FOR HORIZONTAL FOR RETURN FLOW MOMENTUM FLUX\r\n" +
                    "*\r\n" +
                    "C33    IWRU    JWRU    KWRU    IWRD    JWRD    KWRD    QWRE  NQW_RQ  NQWR_U  NQWR_D  BQWR_U  BQWR_D   ANG_D\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C34 TIME CONSTANT WITHDRAWAL AND RETURN CONCENTRATION RISES\r\n" +
                    "*\r\n" +
                    "*  SAL:  SALTINITY RISE\r\n" +
                    "*  TEM:  TEMPERATURE RISE\r\n" +
                    "*  DYE:  DYE CONCENTRATION RISE\r\n" +
                    "*  SFL:  SHELLFISH LARVAE CONCENTRATION RISE\r\n" +
                    "*  TOX#: NTOX TOXIC CONTAMINANT CONCENTRATION RISES\r\n" +
                    "*\r\n" +
                    "C34    SALT    TEMP    DYEC    SFLC    TOX1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C35 TIME CONSTANT WITHDRAWAL AND RETURN CONCENTRATION RISES\r\n" +
                    "*\r\n" +
                    "*  SED#: NSEDC COHESIVE SEDIMENT CONCENTRATION RISE\r\n" +
                    "*  SND#: NSEDN NON-COHESIVE SEDIMENT CONCENTRATION RISE\r\n" +
                    "*\r\n" +
                    "C35   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C36 SEDIMENT INITIALIZATION AND WATER COLUMN/BED REPRESENTATION OPTIONS\r\n" +
                    "*   DATA REQUIRED IF ISTRAN(6) OR ISTRAN(7) <> 0\r\n" +
                    "*\r\n" +
                    "*   ISEDINT: 0 FOR CONSTANT INITIAL CONDITIONS\r\n" +
                    "*            1 FOR SPATIALLY VARIABLE WATER COLUMN INITIAL CONDITIONS\r\n" +
                    "*              FROM SEDW.INP AND SNDW.INP\r\n" +
                    "*            2 FOR SPATIALLY VARIABLE BED INITIAL CONDITIONS\r\n" +
                    "*              FROM SEDB.INP AND SNDB.INP\r\n" +
                    "*            3 FOR SPATIALLY VARIABLE WATER COL AND BED INITIAL CONDITIONS\r\n" +
                    "*  ISEDBINT: 0 FOR SPATIALLY VARYING BED INITIAL CONDITIONS IN MASS/AREA\r\n" +
                    "*            1 FOR SPATIALLY VARYING BED INITIAL CONDITIONS IN MASS FRACTION\r\n" +
                    "*              OF TOTAL SEDIMENT MASS (REQUIRES BED LAYER THICKNESS\r\n" +
                    "*              FILE BEDLAY.INP)\r\n" +
                    "* NSEDFLUME: 0 USE THE SEDIMENT TRANSPORT FUNCTIONS IN EFDC MAIN CODE       \r\n" +
                    "*           98 USE THE SEDZLJ SEDIMENT TRANSPORT SUB-MODEL\r\n" +
                    "*           99 USE THE SEDZLJ SEDIMENT TRANSPORT SUB-MODEL WITH TOXICS\r\n" +
                    "*              \r\n" +
                    "*     ISMUD: 1 INCLUDE COHESIVE FLUID MUD VISCOUS EFFECTS USING EFDC\r\n" +
                    "*              FUNCTION CSEDVIS(SEDT)\r\n" +
                    "*    ISNDWC:   NOT USED\r\n" +
                    "*              \r\n" +
                    "*              \r\n" +
                    "*    ISEDVW: 0 FOR CONSTANT OR SIMPLE CONCENTRATION DEPENDENT\r\n" +
                    "*              COHESIVE SEDIMENT SETTLING VELOCITY\r\n" +
                    "*           >1 CONCENTRATION AND/OR SHEAR/TURBULENCE DEPENDENT COHESIVE\r\n" +
                    "*              SEDIMENT SETTLING VELOCITY. VALUE INDICATES OPTION TO BE USED\r\n" +
                    "*              IN EFDC FUNCTION CSEDSET(SED,SHEAR,ISEDVWC)\r\n" +
                    "*            1 HUANG AND METHA - LAKE OKEECHOBEE\r\n" +
                    "*            2 SHRESTA AND ORLOB - FOR KRONES SAN FRANCISCO BAY DATA\r\n" +
                    "*            3 ZIEGLER AND NESBIT  - FRESH WATER\r\n" +
                    "*           98 LICK FLOCCULATION\r\n" +
                    "*           99 LICK FLOCCULATION WITH FLOC DIAMETER ADVECTION\r\n" +
                    "*    ISNDVW: 0 USE CONSTANT SPECIFIED NON-COHESIVE SED SETTLING VELOCITIES\r\n" +
                    "*              OR CALCULATE FOR CLASS DIAMETER IF SPECIFIED VALUE IS NEG\r\n" +
                    "*           >1 FOLLOW OPTION 0 PROCEDURE BUT APPLY HINDERED SETTLING\r\n" +
                    "*              CORRECTION. VALUE INDICATES OPTION TO BE USED WITH EFDC\r\n" +
                    "*              FUNCTION CSNDSET(SND,SDEN,ISNDVW) VALUE OF ISNDVW INDICATES\r\n" +
                    "*              EXPONENTIAL IN CORRECT (1-SDEN(NS)*SND(NS)**ISNDVW\r\n" +
                    "*        KB:   MAXIMUM NUMBER OF BED LAYERS (EXCLUDING ACTIVE LAYER)\r\n" +
                    "*  ISDTXBUG: 1 TO ACTIVATE SEDIMENT AND TOXICS DIAGNOSTICS\r\n" +
                    "*    \r\n" +
                    "C36 ISEDINT ISEDBINT NSEDFLUME ISMUD  ISNDWC  ISEDVW  ISNDVW      KB  ISDTXBUG\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C36A SEDIMENT INITIALIZATION AND WATER COLUMN/BED REPRESENTATION OPTIONS\r\n" +
                    "*    DATA REQUIRED EVEN IF ISTRAN(6) AND ISTRAN(7) ARE 0\r\n" +
                    "*\r\n" +
                    "* ISBEDSTR:  0 USE HYDRODYNAMIC MODEL STRESS FOR SEDIMENT TRANSPORT\r\n" +
                    "*            1 SEPARATE GRAIN STRESS FROM TOTAL IN COHESIVE AND NON-COHESIVE COMPONENTS\r\n" +
                    "*            2 SEPARATE GRAIN STRESS FROM TOTAL APPLY TO COHESIVE AND NON-COHESIVE SEDS\r\n" +
                    "*            3 USE INDEPENDENT LOG LAW ROUGHNESS HEIGHT FOR SEDIMENT TRANSPORT\r\n" +
                    "*              READ FROM FILE SEDROUGH.INP\r\n" +
                    "*            4 SEPARATE GRAIN STRESS FROM TOTAL USING COHESIVE/NON-COHESIVE WEIGHTED\r\n" +
                    "*              ROUGHNESS AND LOG LAW RESISTANCE (IMPLEMENTED 5/31/05)\r\n" +
                    "*            5 SEPARATE GRAIN STRESS FROM TOTAL USING COHESIVE/NON-COHESIVE WEIGHTED\r\n" +
                    "*              ROUGHNESS AND POWER LAW RESISTANCE (IMPLEMENTED 5/31/05)\r\n" +
                    "* ISBSDIAM:  0 USE D50 DIAMETER FOR NON-COHESIVE ROUGHNESS\r\n" +
                    "*            1 USE 2*D50 FOR NON-COHESIVE ROUGHNESS\r\n" +
                    "*            2 USE D90 FOR NON-COHESIVE ROUGHNESS\r\n" +
                    "*            3 USE 2*D90 FOR NON-COHESIVE ROUGHNESS\r\n" +
                    "* ISBSDFUF:  1 CORRECT GRAIN STRESS PARTITIONING FOR NON-UNIFORM FLOW EFFECTS\r\n" +
                    "*              DO NOT USE FOR ISBEDSTR = 4 AND 5\r\n" +
                    "* COEFTSBL:    COEFFICIENT SPECIFYING THE HYDRODYNAMIC SMOOTHNESS OF\r\n" +
                    "*              TURBULENT BOUNDARY LAYER OVER COHESIVE BED IN TERMS OF\r\n" +
                    "*              EQUIVALENT GRAIN SIZE FOR COHESIVE GRAIN STRESS\r\n" +
                    "*              CALCULATION, FULLY SMOOTH = 4, FULLY ROUGH = 100.\r\n" +
                    "*              NOT USED FOR ISBEDSTR = 4 AND 5\r\n" +
                    "* VISMUDST:    KINEMATIC VISCOSITY TO USE IN DETERMINING COHESIVE GRAIN STRESS\r\n" +
                    "* ISBKERO:   1 FOR BANK EROSION SPECIFIED BY EXTERNAL TIME SERIES\r\n" +
                    "*            2 FOR BANK EROSION INTERNALLY CALCULATED BY STABILITY ANALYSIS (Not Active)\r\n" +
                    "*\r\n" +
                    "C36A ISBEDSTR ISBSDIAM ISBSDFUF COEFTSBL VISMUDST ISBKERO\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C36B SEDIMENT INITIALIZATION AND WATER COLUMN/BED REPRESENTATION OPTIONS\r\n" +
                    "*    DATA REQUIRED EVEN IF ISTRAN(6) AND ISTRAN(7) ARE 0\r\n" +
                    "*\r\n" +
                    "*   ISEDAL:    NOT USED\r\n" +
                    "*   ISNDAL:  1 TO ACTIVATE NON-COHESIVE ARMORING EFFECTS (GARCIA & PARKER)\r\n" +
                    "*            2 SAME AS 1 WITH ACTIVE-PARENT LAYER FORMULATION\r\n" +
                    "*   IALTYP:  0 CONSTANT THICKNESS ARMORING LAYER\r\n" +
                    "*            1 CONSTANT TOTAL SEDIMENT MASS ARMORING LAYER\r\n" +
                    "*   IALSTUP: 1 CREATE ARMORING LAYER FROM INITIAL TOP LAYER AT START UP\r\n" +
                    "*   ISEDEFF: 1 MODIFY NON-COHESIVE RESUSPENSION TO ACCOUNT FOR COHESIVE EFFECTS\r\n" +
                    "*              USING MULTIPLICATION FACTOR: EXP(-COEHEFF*FRACTION COHESIVE)\r\n" +
                    "*            2 MODIFY NON-COHESIVE CRITICAL STRESS TO ACCOUNT FOR COHESIVE EFFECTS\r\n" +
                    "*              USING MULT FACTOR: 1+(COEHEFF2-1)*(1-EXP(-COEHEFF*FRACTION COHESIVE))\r\n" +
                    "*   HBEDAL:    ACTIVE ARMORING LAYER THICKNESS\r\n" +
                    "*   COEHEFF:   COHESIVE EFFECTS COEFFICIENT\r\n" +
                    "*   COEHEFF2:  COHESIVE EFFECTS COEFFICIENT\r\n" +
                    "*\r\n" +
                    "C36B ISEDAL  ISNDAL  IALTYP IALSTUP ISEDEFF  HBEDAL COEHEFF COEHEFF2\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C37 BED MECHANICAL PROPERTIES PARAMETER SET 1\r\n" +
                    "*   DATA REQUIRED IF NSED>0, EVEN IF ISTRAN(6) = 0\r\n" +
                    "*\r\n" +
                    "*  ISEDDT:   NUMBER OF SED/TOX BED PROCESSES STEPS PER HYDRO/WC TRANS STEPS\r\n" +
                    "*  IBMECH: 0 TIME INVARIANT CONSTANT BED MECHANICAL PROPERITES (UNIFORM BED ONLY)\r\n" +
                    "*          1 SIMPLE CONSOLIDATION CALCULATION WITH CONSTANT COEFFICIENTS\r\n" +
                    "*          2 SIMPLE CONSOLIDATION WITH VARIABLE COEFFICIENTS DETERMINED\r\n" +
                    "*            EFDC FUNCTIONS CSEDCON1,2,3(IBMECH)\r\n" +
                    "*          3 COMPLEX CONSOLIDATION WITH VARIABLE COEFFICIENTS DETERMINED\r\n" +
                    "*            EFDC FUNCTIONS CSEDCON1,2,3(IBMECH). IBMECH > 0 SETS THE\r\n" +
                    "*            C38 PARAMETER ISEDBINT=1 AND REQUIRES INITIAL CONDITIONS\r\n" +
                    "*            FILES BEDLAY.INP, BEDBDN.INP AND BEDDDN.IN\r\n" +
                    "*          9 TYPE OF CONSOLIDATION VARIES BY CELL WITH IBMECH FOR EACH\r\n" +
                    "*            DEFINED IN INPUT FILE CONSOLMAP.INP\r\n" +
                    "*  IMORPH: 0 CONSTANT BED MORPHOLOGY (IBMECH=0, ONLY)\r\n" +
                    "*          1 ACTIVE BED MORPHOLOGY: NO WATER ENTRAIN/EXPULSION EFFECTS\r\n" +
                    "*          2 ACTIVE BED MORPHOLOGY: WITH WATER ENTRAIN/EXPULSION EFFECTS\r\n" +
                    "*  HBEDMAX:  TOP BED LAYER THICKNESS (m) AT WHICH NEW LAYER IS ADDED OR IF\r\n" +
                    "*            KBT(I,J)=KB, NEW LAYER ADDED AND LOWEST TWO LAYERS COMBINED\r\n" +
                    "*  BEDPORC:  CONSTANT BED POROSITY (IBMECH=0, OR NSED=0)\r\n" +
                    "*            ALSO USED AS POROSITY OF DEPOSITIN NON-COHESIVE SEDIMENT\r\n" +
                    "*  SEDMDMX:  MAXIMUM FLUID MUD COHESIVE SEDIMENT CONCENTRATION (MG/L)\r\n" +
                    "*  SEDMDMN:  MINIMUM FLUID MUD COHESIVE SEDIMENT CONCENTRATION (MG/L)\r\n" +
                    "*  SEDVDRD:  VOID RATIO OF DEPOSITING COHESIVE SEDIMENT\r\n" +
                    "*  SEDVDRM:  MINIMUM COHESIVE SEDIMENT BED VOID RATIO (IBMECH > 0)\r\n" +
                    "*  SEDVDRT:  BED CONSOLIDATION RATE CONSTANT (sec) (IBMECH = 1,2), EXP(-DELT/SEDVDRT) \r\n" +
                    "*              > 0 CONSOLIDATE OVER TIME TO SEDVDRM\r\n" +
                    "*              = 0 CONSOLIDATE INSTANTANEOUSLY TO SEDVDRM (0.0>=SEDVDRT<=0.0001)\r\n" +
                    "*              < 0 CONSOLIDATE TO INITIAL VOID RATIOS\r\n" +
                    "*\r\n" +
                    "C37  ISEDDT  IBMECH  IMORPH HBEDMAX BEDPORC SEDMDMX SEDMDMN SEDVDRD SEDVDRM SEDVRDT\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C38 BED MECHANICAL PROPERTIES PARAMETER SET 2\r\n" +
                    "*   DATA REQUIRED IF NSED>0, EVEN IF ISTRAN(6) = 0\r\n" +
                    "*\r\n" +
                    "*  IBMECHK: 0 FOR HYDRAULIC CONDUCTIVITY, K, FUNCTION K=KO*EXP((E-EO)/EK)\r\n" +
                    "*           1 FOR HYD COND/(1+VOID RATIO),K', FUNCTION K'=KO'*EXP((E-EO)/EK)\r\n" +
                    "*   BMECH1:   REFERENCE EFFECTIVE STRESS/WATER SPECIFIC WEIGHT, SEO (m)\r\n" +
                    "*             IF BMECH1<0 USE INTERNAL FUNCTION, BMECH1,BMECH2,BMECH3 NOT USED\r\n" +
                    "*   BMECH2:   REFERENCE VOID RATIO FOR EFFECTIVE STRESS FUNCTION, EO\r\n" +
                    "*   BMECH3:   VOID RATIO RATE TERM ES IN SE=SEO*EXP(-(E-EO)/ES)\r\n" +
                    "*   BMECH4:   REFERENCE HYDRAULIC CONDUCTIVITY, KO (m/s)\r\n" +
                    "*             IF BMECH4<0 USE INTERNAL FUNCTION, BMECH1,BMECH2,BMECH3 NOT USED\r\n" +
                    "*   BMECH5:   REFERENCE VOID RATIO FOR HYDRAULIC CONDUCTIVITY, EO\r\n" +
                    "*   BMECH6:   VOID RATIO RATE TERM EK IN (K OR K')=(KO OR KO')*EXP((E-EO)/EK)\r\n" +
                    "*\r\n" +
                    "C38 IBMECHK  BMECH1  BMECH2  BMECH3  BMECH4  BMECH5  BMECH6\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C39 COHESIVE SEDIMENT PARAMETER SET 1 REPEAT DATA LINE NSED TIMES\r\n" +
                    "*   DATA REQUIRED IF NSED>0, EVEN IF ISTRAN(6) = 0\r\n" +
                    "*\r\n" +
                    "*  SEDO:   CONSTANT INITIAL COHESIVE SEDIMENT CONC IN WATER COLUMN\r\n" +
                    "*           (MG/LITER=GM/M**3)\r\n" +
                    "*  SEDBO:  CONSTANT INITIAL COHESIVE SEDIMENT IN BED PER UNIT AREA\r\n" +
                    "*           (GM/SQ METER)  IE 1CM THICKNESS BED WITH SSG=2.5 AND\r\n" +
                    "*           N=.6,.5 GIVES SEDBO 1.E4, 1.25E4\r\n" +
                    "*  SDEN:   SEDIMENT SPEC VOLUME (IE 1/2.25E6 M**3/GM)\r\n" +
                    "*  SSG:    SEDIMENT SPECIFIC GRAVITY\r\n" +
                    "*  WSEDO:  CONSTANT OR REFERENCE SEDIMENT SETTLING VELOCITY\r\n" +
                    "*            IN FORMULA WSED=WSEDO*( (SED/SEDSN)**SEXP )\r\n" +
                    "*  SEDSN:  (NOT USED)\r\n" +
                    "*  SEXP:   (NOT USED)\r\n" +
                    "*  TAUD:   BOUNDARY STRESS BELOW WHICH DEPOSITION TAKES PLACE ACCORDING\r\n" +
                    "*           TO (TAUD-TAU)/TAUD\r\n" +
                    "*  ISEDSCOR:  1 TO CORRECT BOTTOM LAYER CONCENTRATION TO NEAR BED CONCENTRATION\r\n" +
                    "*  ISPROBDEP: 0 KRONE PROBABILITY OF DEPOSITION USING COHESIVE GRAIN STRESS\r\n" +
                    "*             1 KRONE PROBABILITY OF DEPOSITION USING TOTAL BED STRESS\r\n" +
                    "*             2 PARTHENIADES PROBABILITY OF DEPOSITION USING COHESIVE GRAIN STRESS\r\n" +
                    "*             3 PARTHENIADES PROBABILITY OF DEPOSITION USING TOTAL BED STRESS\r\n" +
                    "*\r\n" +
                    "C39    SEDO   SEDBO    SDEN     SSG   WSEDO   SEDSN    SEXP    TAUD ISEDSCOR ISPROBDEP\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C40 COHESIVE SEDIMENT PARAMETER SET 2 REPEAT DATA LINE NSED TIMES\r\n" +
                    "*   DATA REQUIRED IF NSED>0, EVEN IF ISTRAN(6) = 0\r\n" +
                    "*\r\n" +
                    "*  IWRSP: 0 USE RESUSPENSION RATE AND CRITICAL STRESS BASED ON PARAMETERS\r\n" +
                    "*           ON THIS DATA LINE\r\n" +
                    "*        >0 USE BED PROPERTIES DEPENDEDNT RESUSPENSION RATE AND CRITICAL\r\n" +
                    "*           STRESS GIVEN BY EFDC FUNCTIONS CSEDRESS,CSEDTAUS,CSEDTAUB\r\n" +
                    "*           FUNCTION ARGUMENSTS ARE (BDENBED,IWRSP)\r\n" +
                    "*         1 HWANG AND METHA - LAKE OKEECHOBEE\r\n" +
                    "*         2 HAMRICK'S MODIFICATION OF SANFORD AND MAA\r\n" +
                    "*         3 SAME AS 2 EXCEPT VOID RATIO OF COHESIVE SEDIMENT FRACTION IS USED\r\n" +
                    "*         4 SEDFLUME WITHOUT CRITICAL STRESS\r\n" +
                    "*         5 SEDFLUME WITH CRITICAL STRESS\r\n" +
                    "*     >= 99 SITE SPECIFIC\r\n" +
                    "*  IWRSPB:0 NO BULK EROSION\r\n" +
                    "*         1 USE BULK EROSION CRITICAL STRESS AND RATE IN FUNCTIONS\r\n" +
                    "*           CSEDTAUB AND CSEDRESSB\r\n" +
                    "*  WRSPO:   REF SURFACE EROSION  RATE IN FORMULA\r\n" +
                    "*           WRSP=WRSP0*( ((TAU-TAUR)/TAUN)**TEXP ) (gm/m**2/sec)\r\n" +
                    "*  TAUR:    BOUNDARY STRESS ABOVE WHICH SURFACE EROSION OCCURS (m/s)**2\r\n" +
                    "*  TAUN:    (NOT USED, TAUN=TAUR SET IN CODE)\r\n" +
                    "*  TEXP:    EXPONENT OF   WRSP=WRSP0*( ((TAU-TAUR)/TAUN)**TEXP )\r\n" +
                    "*  VDRRSPO: REFERENCE VOID RATIO FOR CRITICAL STRESS AND RESUSPENSION RATE\r\n" +
                    "*            IWRSP=2,3\r\n" +
                    "* COSEDHID: COHESIVE SEDIMENT RESUSPENSION HIDING FACTOR TO REDUCE COHESIVE\r\n" +
                    "*           RESUSPENSION BY FACTOR = (COHESIVE FRACTION OF SEDIMENT)**COSEDHID\r\n" +
                    "*\r\n" +
                    "C40   IWRSP  IWRSPB   WRSPO    TAUR    TAUN    TEXP VDRRSPO COSEDHID\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C41 NON-COHESIVE SEDIMENT PARAMETER SET 1 REPEAT DATA LINE NSND TIMES\r\n" +
                    "*    DATA REQUIRED IF NSND>0, EVEN IF ISTRAN(7) = 0\r\n" +
                    "*\r\n" +
                    "*  SNDO:     CONSTANT INITIAL NON-COHESIVE SEDIMENT CONC IN WATER COLUMN\r\n" +
                    "*             (MG/LITER=GM/M**3)\r\n" +
                    "*  SNDBO:    CONSTANT INITIAL NON-COHESIVE SEDIMENT IN BED PER UNIT AREA\r\n" +
                    "*             (GM/SQ METER)  IE 1CM THICKNESS BED WITH SSG=2.5 AND\r\n" +
                    "*             N=.6,.5 GIVES SNDBO 1.E4, 1.25E4\r\n" +
                    "*  SDEN:     SEDIMENT SPEC VOLUME (IE 1/2.65E6 M**3/GM)\r\n" +
                    "*  SSG:      SEDIMENT SPECIFIC GRAVITY\r\n" +
                    "*  SNDDIA:   REPRESENTATIVE DIAMETER OF SEDIMENT CLASS (m)\r\n" +
                    "*  WSNDO:    CONSTANT OR REFERENCE SEDIMENT SETTLING VELOCITY\r\n" +
                    "*            WSNDO < 0, SETTLING VELOCITY INTERNALLY COMPUTED\r\n" +
                    "*  SNDN:     (NOT USED)\r\n" +
                    "*  SEXP:     (NOT USED)\r\n" +
                    "*  TAUD:     (NOT USED)\r\n" +
                    "*  ISNDSCOR: (NOT USED)\r\n" +
                    "*\r\n" +
                    "C41    SNDO   SNDBO    SDEN     SSG  SNDDIA   WSNDO    SNDN    SEXP    TAUD ISNDSCOR\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C42 NON-COHESIVE SEDIMENT PARAMETER SET 2 REPEAT DATA LINE NSND TIMES\r\n" +
                    "*    DATA REQUIRED IF NSND>0, EVEN IF ISTRAN(7) = 0\r\n" +
                    "*\r\n" +
                    "*    ISNDEQ: >1 CALCULATE ABOVE BED REFERENCE NON-COHESIVE SEDIMENT\r\n" +
                    "*               EQUILIBRIUM CONCENTRATION USING EFDC FUNCTION\r\n" +
                    "*               CSNDEQC(SNDDIA,SSG,WS,TAUR,TAUB,SIGPHI,SNDDMX,IOTP)\r\n" +
                    "*               WHICH IMPLEMENT FORMULATIONS OF\r\n" +
                    "*             1 GARCIA AND PARKER\r\n" +
                    "*             2 SMITH AND MCLEAN\r\n" +
                    "*             3 VAN RIJN\r\n" +
                    "*             4 SEDFLUME WITHOUT CRITICAL STRESS\r\n" +
                    "*             5 SEDFLUME WITH CRITICAL STRESS\r\n" +
                    "*    ISBDLD:  0 BED LOAD PHI FUNCTION IS CONSTANT, SBDLDP\r\n" +
                    "*             1 VAN RIJN PHI FUNCTION\r\n" +
                    "*             2 MODIFIED ENGULAND-HANSEN\r\n" +
                    "*             3 WU, WANG, AND JIA\r\n" +
                    "*             4 (NOT USED)\r\n" +
                    "*             5 (NOT USED)\r\n" +
                    "*     TAUR:     CRITICAL STRESS IN (m/s)**2 \r\n" +
                    "*               NOTE: IF TAUR < 0, THEN TAUR AND TAUN ARE INTERNALLY\r\n" +
                    "*               COMPUTED USING VAN RIJN'S FORMULAS\r\n" +
                    "*     TAUN:     EQUAL TO TAUR FOR NON-COHESIVE SED TRANS\r\n" +
                    "* TCSHIELDS:    CRITICAL SHIELDS STRESS (DIMENSIONLESS)\r\n" +
                    "*   ISLTAUC:  1 TO IMPLEMENT SUSP LOAD ONLY WHEN STRESS EXCEEDS TAUC FOR EACH GRAINSIZE\r\n" +
                    "*             2 TO IMPLEMENT SUSP LOAD ONLY WHEN STRESS EXCEEDS TAUCD50\r\n" +
                    "*             3 TO USE TAUC FOR NONUNIFORM BEDS, THESE APPLY ONLY TO RESUSPENSION\r\n" +
                    "*               FORMULAS NOT EXPLICITLY CONTAINING CRITICAL SHIELDS STRESS SUCH AS G-P\r\n" +
                    "*   IBLTAUC:  1 TO IMPLEMENT BEDLOAD ONLY WHEN STRESS EXCEEDS TAUC FOR EACH GRAINSIZE\r\n" +
                    "*             2 TO IMPLEMENT BEDLOAD ONLY WHEN STRESS EXCEEDS TAUCD50\r\n" +
                    "*             3 TO USE TAUC FOR NONUNIFORM BEDS, THESE APPLY ONLY TO BED LOAD\r\n" +
                    "*               FORMULAS NOT EXPLICITLY CONTAINING CRITICAL SHIELDS STRESS SUCH AS E-H\r\n" +
                    "*    IROUSE:  0 USE TOTAL STRESS FOR CALCULATING ROUSE NUMBER\r\n" +
                    "*             1 USE GRAIN STRESS FOR ROUSE NUMBER\r\n" +
                    "*    ISNDM1:  0 SET BOTH BEDLOAD AND SUSPENDED LOAD FRACTIONS TO 1.0\r\n" +
                    "*             1 SET BEDLOAD FRACTION TO 1. USE BINARY RELATIONSHIP FOR SUSPENDED\r\n" +
                    "*             2 SET BEDLOAD FRACTION TO 1, USE LINEAR RELATIONSHIP FOR SUSPENDED\r\n" +
                    "*             3 USE BINARY RELATIONSHIP FOR BEDLOAD AND SUSPENDED LOAD\r\n" +
                    "*             4 USE LINEAR RELATIONSHIP FOR BEDLOAD AND SUSPENDED LOAD\r\n" +
                    "*    ISNDM2:  0 USE TOTAL SHEAR VELOCITY IN USTAR/WSET RATIO\r\n" +
                    "*             1 USE GRAIN SHEAR VELOCITY IN USTAR/WSET RATIO\r\n" +
                    "*     RSNDM:    VALUE OF USTAR/WSET FOR BINARY SWITCH BETWEEN BEDLOAD AND SUSPENDED LOAD\r\n" +
                    "*\r\n" +
                    "C42 ISNDEQ ISBDLD   TAUR       TAUN    TCSHIELDS ISLTAUC IBLTAUC IROUSE  ISNDM1  ISNDM2   RSNDM\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C42A NON-COHESIVE SEDIMENT PARAMETER SET 3 (BED LOAD FORMULA PARAMETERS)\r\n" +
                    "*    DATA REQUIRED IF NSND>0, EVEN IF ISTRAN(7) = 0\r\n" +
                    "*                                                                      \r\n" +
                    "*   ISBDLDBC:  0 DISABLE BEDLOAD       \r\n" +
                    "*              1 ACTIVATE BEDLOAD OPTION.  MUST USE SEDBLBC.INP\r\n" +
                    "*     SBDLDA:    ALPHA EXPONENTIAL FOR BED LOAD FORMULA \r\n" +
                    "*     SBDLDB:    BETA EXPONENTIAL FOR BED LOAD FORMULA\r\n" +
                    "*    SBDLDG1:    GAMMA1 CONSTANT FOR BED LOAD FORMULA \r\n" +
                    "*    SBDLDG2:    GAMMA2 CONSTANT FOR BED LOAD FORMULA \r\n" +
                    "*    SBDLDG3:    GAMMA3 CONSTANT FOR BED LOAD FORMULA\r\n" +
                    "*    SBDLDG4:    GAMMA4 CONSTANT FOR BED LOAD FORMULA\r\n" +
                    "*     SBDLDP:    CONSTANT PHI FOR BED LOAD FORMULA \r\n" +
                    "*    ISBLFUC:    BED LOAD FACE FLUX , 0 FOR DOWN WIND PROJECTION,1 FOR DOWN WIND\r\n" +
                    "*                WITH CORNER CORRECTION,2 FOR CENTERED AVERAGING\r\n" +
                    "*     BLBSNT:    ADVERSE BED SLOPE (POSITIVE VALUE) ACROSS A CELL FACE ABOVE\r\n" +
                    "*                WHICH NO BED LOAD TRANSPORT CAN OCCUR.  NOT ACTIVE FOR BLBSNT=0.0\r\n" +
                    "*\r\n" +
                    "C42a IBEDLD  SBDLDA  SBDLDB SBDLDG1 SBDLDG2 SBDLDG3 SBDLDG4  SBDLDP ISBLFUC  BLBSNT\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C43 TOXIC CONTAMINANT INITIAL CONDITIONS AND PARAMETERS\r\n" +
                    "*  USER MAY CHANGE UNITS OF WATER AND SED PHASE TOX CONCENTRATION\r\n" +
                    "*  AND PARTIATION COEFFICIENT ON C44 - C46 BUT CONSISTENT UNITS MUST\r\n" +
                    "*  MUST BE USED FOR MEANINGFUL RESULTS\r\n" +
                    "*  DATA REQUIRED EVEN IT ISTRAN(5) IS 0\r\n" +
                    "*\r\n" +
                    "*    NTOXN: TOXIC CONTAMINANT NUMBER ID (1 LINE OF DATA BY DEFAULT)\r\n" +
                    "*   ITXINT: 0 FOR SPATIALLY CONSTANT WATER COL AND BED INITIAL CONDITIONS\r\n" +
                    "*           1 FOR SPATIALLY VARIABLE WATER COLUMN INITIAL CONDITIONS\r\n" +
                    "*           2 FOR SPATIALLY VARIABLE BED INITIAL CONDITIONS\r\n" +
                    "*           3 FOR SPATIALLY VARIABLE WATER COL AND BED INITIAL CONDITION\r\n" +
                    "*  ITXBDUT: SET TO 0 FOR INITIAL BED GIVEN BY TOTAL TOX (mg/m^3)\r\n" +
                    "*           SET TO 1 FOR INITIAL BED GIVEN BY SORBED MASS TOX/MASS SED(mg/kg)\r\n" +
                    "*  TOXINTW: INIT WATER COLUNM TOT TOXIC VARIABLE CONCENTRATION (ugm/l)\r\n" +
                    "*  TOXINTB: INIT SED BED TOXIC CONC SEE ITXBDUT\r\n" +
                    "*             \r\n" +
                    "*   RKTOXW: FIRST ORDER WATER COL DECAY RATE FOR TOX VARIABLE IN 1/sec\r\n" +
                    "*   TKTOXW: REF TEMP FOR 1ST ORDER WATER COL DECAY DEG C\r\n" +
                    "*   RKTOXB: FIRST ORDER SED BED DECAY RATE FOR TOX VARIABLE IN 1/sec\r\n" +
                    "*   TKTOXB: REF TEMP FOR 1ST ORDER SED BED DECAY DEG C\r\n" +
                    "*\r\n" +
                    "C43   NTOXN  ITXINT ITXBDUT TOXINTW TOXINTB  RKTOXW  TKTOXW  RKTOXB  TRTOXB COMMENTS\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C44 ADDITIONAL TOXIC CONTAMINANT PARAMETERS\r\n" +
                    "*   DATA REQUIRED EVEN IT ISTRAN(5) IS 0\r\n" +
                    "*\r\n" +
                    "*    NTOXN: TOXIC CONTAMINANT NUMBER ID (1 LINE OF DATA BY DEFAULT)\r\n" +
                    "*    ISTOC: 1 FOR DISS AND PART ORGANIC CARBON SORPTION\r\n" +
                    "*           2 FOR DISS ORGANIC CARBON SORPTION AND POC FRACTIONALLY\r\n" +
                    "*             DISTRIBUTED TO INORGANIC SEDIMENT CLASSES\r\n" +
                    "*           3 FOR NO DISS ORGANIC CARBON SORPTION AND POC FRACTIONALLY\r\n" +
                    "*             DISTRIBUTED TO INORGANIC SEDIMENT CLASSES\r\n" +
                    "*   VOLTOX:   WATER SURFACE VOLITIALIZATION RATE MULTIPLIER (0. OR 1.)\r\n" +
                    "*   RMOLTX:   MOLECULAR WEIGHT FOR DETERMINING VOLATILIZATION RATE\r\n" +
                    "*   RKTOXP:   REFERENCE PHOTOLOYSIS DECAY RATE 1/sec\r\n" +
                    "*   SKTOXP:   REFERENCE SOLAR RADIATION FOR PHOTOLOYSIS (watts/m**2)\r\n" +
                    "*   DIFTOX:   DIFFUSION COEFF FOR TOXICANT IN SED BED PORE WATER (m**2/s)\r\n" +
                    "*   DIFTOXS:  DIFFUSION COEFF FOR TOXICANT BETWEEN WATER COLUMN AND\r\n" +
                    "*              PORE WATER IN TOP LAYER OF THE BED(m**2/s)\r\n" +
                    "*              > 0.0 INTERPRET AS DIFFUSION COEFFICIENT (m**2/s)\r\n" +
                    "*              < 0.0 INTERPRET AS FLUX VELOCITY (m/s)\r\n" +
                    "*   PDIFTOX:  PARTICLE MIXING DIFFUSION COEFF FOR TOXICANT IN SED BED (m**2/s)\r\n" +
                    "*             (if negative use zonal files PARTMIX.INP and PMXMAP.INP)\r\n" +
                    "*   DPDIFTOX: DEPTH IN BED OVER WHICH PARTICLE MIXING IS ACTIVE (m)\r\n" +
                    "*\r\n" +
                    "C44   NTOXN   ISTOC  VOLTOX  RMOLTX  RKTOXP  SKTOXP  DIFTOX DIFTOXS PDIFTOX DPDIFTOX\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C45 TOXIC CONTAMINANT SEDIMENT INTERACTION PARAMETERS\r\n" +
                    "*   \r\n" +
                    "*\r\n" +
                    "*    NTOXC: TOXIC CONTAMINANT NUMBER ID.  NSEDC+NSEDN LINES OF DATA\r\n" +
                    "*             FOR EACH TOXIC CONTAMINANT (DEFAULT = 2)\r\n" +
                    "* NSEDN/NSNDN: FIRST NSED LINES COHESIVE, NEXT NSND LINES NON-COHESIVE.\r\n" +
                    "*           REPEATED FOR EACH CONTAMINANT\r\n" +
                    "*  ITXPARW: EQUAL 1 FOR SOLIDS DEPENDENT PARTITIONING (WC) GIVEN BY\r\n" +
                    "*           TOXPAR=PARO*(CSED**CONPAR)\r\n" +
                    "*  TOXPARW: WATER COLUMN PARO (ITXPARW=1) OR EQUIL TOX CON PART COEFF BETWEEN\r\n" +
                    "*           EACH TOXIC IN WATER AND ASSOCIATED SEDIMENT PHASES (LITERS/MG)\r\n" +
                    "*  CONPARW: EXPONENT IN  TOXPAR=PARO*(CSED**CONPARW) IF ITXPARW=1\r\n" +
                    "*  ITXPARB: Not Used\r\n" +
                    "*  TOXPARB: SEDIMENT BED PARO (ITXPARB=1) OR EQUIL TOX CON PART COEFF BETWEEN\r\n" +
                    "*           EACH TOXIC IN WATER AND ASSOCIATED SEDIMENT PHASES (LITERS/MG)\r\n" +
                    "*  CONPARB: Not Used\r\n" +
                    "*                      1     0.8770  -0.943          0.025\r\n" +
                    "C45   NTOXN   NSEDN ITXPARW TOXPARW CONPARW ITXPARB TOXPARB CONPARB\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C45A TOXIC CONTAMINANT ORGANIC CARBON INTERACTION PARAMETERS\r\n" +
                    "*\r\n" +
                    "* ISTDOCW: 0 CONSTANT DOC IN WATER COLUMN OF STDOCWC (DEFAULT=0.)\r\n" +
                    "*          1 TIME CONSTANT, SPATIALLY VARYING DOC IN WATER COLUMN FROM docw.inp\r\n" +
                    "* ISTPOCW: 0 CONSTANT POC IN WATER COLUMN OF STPOCWC (DEFAULT=0.)\r\n" +
                    "*          1 TIME CONSTANT, SPATIALLY VARYING POC IN WATER COLUMN FROM pocw.inp\r\n" +
                    "*          2 TIME CONSTANT, FPOC IN WATER COLUMN, SEE C45C\r\n" +
                    "*          3 TIME CONSTANT, SPATIALLY VARYING FPOC IN WATER COLUMN FORM fpocw.inp\r\n" +
                    "*          4 FUNTIONAL SPECIFICATION OF TIME AND SPATIALLY VARYING\r\n" +
                    "*              FPOC IN WATER COLUMN\r\n" +
                    "* ISTDOCB: 0 CONSTANT DOC IN BED OF STDOCBC (DEFAULT=0.)\r\n" +
                    "*          1 TIME CONSTANT, SPATIALLY VARYING DOC IN BED FROM docb.inp\r\n" +
                    "* ISTPOCB: 0 CONSTANT POC IN BED OF STPOCBC (DEFAULT=0.)\r\n" +
                    "*          1 TIME CONSTANT, SPATIALLY VARYING POC IN BED FROM pocb.inp\r\n" +
                    "*          2 TIME CONSTANT, FPOC IN BED, SEE C45D\r\n" +
                    "*          3 TIME CONSTANT, SPATIALLY VARYING FPOC IN BED FROM fpocb.inp\r\n" +
                    "*          4 FUNTIONAL SPECIFICATION OF TIME AND SPATIALLY VARYING\r\n" +
                    "*              FPOC IN BED\r\n" +
                    "* STDOCWC:   CONSTANT WATER COLUMN DOC (ISTDOCW=0)\r\n" +
                    "* STPOCWC:   CONSTANT WATER COLUMN POC (ISTPOCW=0)\r\n" +
                    "* STDOCBC:   CONSTANT BED DOC (ISTDOCB=0)\r\n" +
                    "* STPOCBC:   CONSTANT BED POC (ISTPOCB=0)\r\n" +
                    "*\r\n" +
                    "C45A ISTDOCW ISTPOCW ISTDOCB ISTPOCB STDOCWC STPOCWC STDOCBC STPOCBC\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C45B TOXIC CONTAMINANT ORGANIC CARBON (OC) INTERACTION PARAMETERS\r\n" +
                    "*   \r\n" +
                    "*\r\n" +
                    "*    NTOXC: TOXIC CONTAMINANT NUMBER ID.  NSEDC+NSEDN LINES OF DATA\r\n" +
                    "*             FOR EACH TOXIC CONTAMINANT (DEFAULT = 2)\r\n" +
                    "*     NOC : FIRST LINE FOR DISSOLVED ORGANIC CARBON, SECOND LINE FOR PARTICULATE OC\r\n" +
                    "*            REPEATED FOR EACH CONTAMINANT\r\n" +
                    "*  ITXPARWC: -1 FOR NO ORGANIC CARBON, O FOR NORMAL PARTITION AND 1 FOR SOLIDS\r\n" +
                    "*            DEPENDENT TOXPAR=PARO*(CSED**CONPAR)\r\n" +
                    "*  TOXPARWC: WATER COLUMN PARO (ITXPARW=1) OR EQUIL TOX CON PART COEFF BETWEEN\r\n" +
                    "*            EACH TOXIC IN WATER AND ASSOCIATED SEDIMENT PHASES (liters/mg)\r\n" +
                    "*  CONPARWC: EXPONENT IN  TOXPAR=PARO*(CSED**CONPARW) IF ITXPARW=1\r\n" +
                    "*  ITXPARBC: Not Used\r\n" +
                    "*  TOXPARBC: SEDIMENT BED PARO (ITXPARB=1) OR EQUIL TOX CON PART COEFF BETWEEN\r\n" +
                    "*            EACH TOXIC IN WATER AND ASSOCIATED SEDIMENT PHASES (liters/mg)\r\n" +
                    "*  CONPARBC: Not Used\r\n" +
                    "*               1       0.8770  -0.943          0.025\r\n" +
                    "C45B   NTOXN     NOC ITXPARWC TOXPARWC CONPARWC ITXPARBC TOXPARBC CONPARBC  *CARBON*\r\n" +
                    " -------------------------------------------------------------------------------\r\n" +
                    "C45C TOXIC CONTAMINANT POC FRACTIONAL DISTRIBUTIONS IN WATER COLUMN\r\n" +
                    "*    1 LINE  OF DATA REQUIRED EVEN IT ISTRAN(5) IS 0. DATA USED WHEN\r\n" +
                    "*      ISTOC(NT)=1 OR 2\r\n" +
                    "*\r\n" +
                    "*     NTOXN:      TOXIC CONTAMINANT NUMBER ID.  NSEDC+NSEDN 1 LINE OF DATA\r\n" +
                    "*                 FOR EACH TOXIC CONTAMINANT (DEFAULT = 2)\r\n" +
                    "*  FPOCSED1-NSED: FRACTION OF OC ASSOCIATED WITH SED CLASSES 1,NSED\r\n" +
                    "*  FPOCSND1-NSND: FRACTION OF OC ASSOCIATED WITH SND CLASSES 1,NSND\r\n" +
                    "*\r\n" +
                    "C45C   NTOXN\r\n" +
                    " -------------------------------------------------------------------------------\r\n" +
                    "C45D TOXIC CONTAMINANT POC FRACTIONAL DISTRIBUTIONS IN SEDIMENT BED\r\n" +
                    "*   1 LINE  OF DATA REQUIRED EVEN IT ISTRAN(5) IS 0. DATA USED WHEN\r\n" +
                    "*   ISTOC(NT)=1 OR 2\r\n" +
                    "*\r\n" +
                    "*     NTOXN:      TOXIC CONTAMINANT NUMBER ID.  NSEDC+NSEDN 1 LINE OF DATA\r\n" +
                    "*                FOR EACH TOXIC CONTAMINANT (DEFAULT = 2)\r\n" +
                    "* FPOCSED1-NSED: FRACTION OF OC ASSOCIATED WITH SED CLASSES 1,NSED\r\n" +
                    "* FPOCSND1-NSND: FRACTION OF OC ASSOCIATED WITH SND CLASSES 1,NSND\r\n" +
                    "*\r\n" +
                    "C45D   NTOXN\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C46 BUOYANCY, TEMPERATURE, DYE DATA AND CONCENTRATION BC DATA\r\n" +
                    "*\r\n" +
                    "* BSC:         BUOYANCY INFLUENCE COEFFICIENT 0 TO 1, BSC=1. FOR REAL PHYSICS\r\n" +
                    "* TEMO:        REFERENCE, INITIAL, EQUILIBRUM AND/OR ISOTHERMAL TEMP IN DEG C\r\n" +
                    "* HEQT:        EQUILIBRUM TEMPERTURE TRANSFER COEFFICIENT M/sec\r\n" +
                    "* ISBEDTEMI: NOT USED\r\n" +
                    "*            \r\n" +
                    "* KBH:       NOT USED\r\n" +
                    "* RKDYE:       FIRST ORDER DECAY RATE FOR DYE VARIABLE IN 1/sec\r\n" +
                    "* NCBS:        NUMBER OF CONCENTRATION BOUNDARY CONDITIONS ON SOUTH OPEN\r\n" +
                    "*               BOUNDARIES\r\n" +
                    "* NCBW:        NUMBER OF CONCENTRATION BOUNDARY CONDITIONS ON WEST OPEN\r\n" +
                    "*               BOUNDARIES\r\n" +
                    "* NCBE:        NUMBER OF CONCENTRATION BOUNDARY CONDITIONS ON EAST OPEN\r\n" +
                    "*               BOUNDARIES\r\n" +
                    "* NCBN:        NUMBER OF CONCENTRATION BOUNDARY CONDITIONS ON NORTH OPEN\r\n" +
                    "*               BOUNDARIES\r\n" +
                    "*\r\n" +
                    "C46    BSC  TEMO    HEQT   ISBEDTEMI KBH     RKDYE   NCBS NCBW NCBE NCBN\r\n");
            writer.write("         1  -1.0  0.000E+00       1    2  " + map.get("C46_RKDYE") + "    0    0    0    0\r\n");
            writer.write("-------------------------------------------------------------------------------\r\n" +
                    "C46A  ICE EFFECTS\r\n" +
                    "C\r\n" +
                    "      ISICE:       1 FOR ICE EFFECTS ACTIVE\r\n" +
                    "      ISICECOV:    0 USE START AND STOP JULIAN DAYS\r\n" +
                    "                   1 READ ICE COVER FROM FILE ICECOVER.INP\r\n" +
                    "                   2 SIMPLE CALCUATION USING AIR TEMPERATURE\r\n" +
                    "                   3 COMPLEX CALCULATION(Not Active)\r\n" +
                    "      ISICETHK:    0 USE START AND STOP JULIAN DAYS AND MAX ICE THICKNESS\r\n" +
                    "                   1 READ ICE THICKNESS FROM FILE ICECOVER.INP\r\n" +
                    "                   2 SIMPLE CALCUATION USING AIR TEMPERATURE\r\n" +
                    "                   3 COMPLEX CALCULATION (Not Active)\r\n" +
                    "      NISER:         NUMBER OF ICE TIME SERIES\r\n" +
                    "      ICETHKFUN:   0 CONSTANT AT RICETHKMAX\r\n" +
                    "                   1 LINEAR TO DYICEM1, LINEAR FROM DYICEM2\r\n" +
                    "                   2 HALF COS WAVE TO TO DYICEM1,  HALF COS FROM DYICEM2\r\n" +
                    "      DYICEBEG:      DAY ICE COVER BEGINS\r\n" +
                    "      DYICEEND:      DAY ICE COVER ENDS\r\n" +
                    "      DYICEM1:       DAY MAXIMUM ICE COVER IS REACHED\r\n" +
                    "      DYICEM2:       DAY MAXIMUM ICE THICKNESS STARTS TO DECAY\r\n" +
                    "      RICETHKMAX:    MAX ICE COVER THICKNESS, METERS\r\n" +
                    "      TEMPICE:       WATER TEMPERATURE AT WATER ICE INTERFACE FOR ISICECOV.LE.2\r\n" +
                    "C\r\n" +
                    "C46A ISICE ISICECOV ISICETHK NISER ICETHKFUN DYICEBEG DYICEEND DYICEM1 DYICEM2 RICETHKMAX TEMPICE\r\n" +
                    "          0       0       1       0       0       0       1       0       0       1      .1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C47 LOCATION OF CONC BC'S ON SOUTH BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  ICBS:    I CELL INDEX\r\n" +
                    "*  JCBS:    J CELL INDEX\r\n" +
                    "*  NTSCRS:  NUMBER OF TIME STEPS TO RECOVER SPECIFIED VALUES ON CHANGE\r\n" +
                    "*           TO INFLOW FROM OUTFLOW\r\n" +
                    "*  NSSERS:  SOUTH BOUNDARY CELL SALINITY TIME SERIES ID NUMBER\r\n" +
                    "*  NTSERS:  SOUTH BOUNDARY CELL TEMPERATURE TIME SERIES ID NUMBER\r\n" +
                    "*  NDSERS:  SOUTH BOUNDARY CELL DYE CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSFSERS: SOUTH BOUNDARY CELL SHELLFISH LARVAE TIME SERIES ID NUMBER\r\n" +
                    "*  NTXSERS: SOUTH BOUNDARY CELL TOXIC CONTAMINANT CONC TIME SERIES ID NUM.\r\n" +
                    "*  NSDSERS: SOUTH BOUNDARY CELL COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSNSERS: SOUTH BOUNDARY CELL NON-COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "C\r\n" +
                    "C47   IBBS JBBS  NTSCRS  NSSERS  NTSERS  NDSERS NSFSERS NTXSERS NSDSERS NSNSERS\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C48 TIME CONSTANT BOTTOM CONC ON SOUTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING BOTTOM LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING BOTTOM LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING BOTTOM LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING BOTTOM LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING BOTTOM LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C48    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C49 TIME CONSTANT BOTTOM CONC ON SOUTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING BOTTOM LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING BOTTOM LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C49   SED1    SND1\r\n" +
                    "------------------------------------------------------------------------------\r\n" +
                    "C50 TIME CONSTANT SURFACE CONC ON SOUTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING SURFAC LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING SURFAC LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING SURFAC LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING SURFAC LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING SURFAC LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C50    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C51 TIME CONSTANT SURFACE CONC ON SOUTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING SURFAC LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING SURFAC LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C51   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C52 LOCATION OF CONC BC'S ON WEST BOUNDARIES AND SERIES IDENTIFIERS\r\n" +
                    "*\r\n" +
                    "*  ICBW:    I CELL INDEX\r\n" +
                    "*  JCBW:    J CELL INDEX\r\n" +
                    "*  NTSCRW:  NUMBER OF TIME STEPS TO RECOVER SPECIFIED VALUES ON CHANGE\r\n" +
                    "*           TO INFLOW FROM OUTFLOW\r\n" +
                    "*  NSSERW:  WEST BOUNDARY CELL SALINITY TIME SERIES ID NUMBER\r\n" +
                    "*  NTSERW:  WEST BOUNDARY CELL TEMPERATURE TIME SERIES ID NUMBER\r\n" +
                    "*  NDSERW:  WEST BOUNDARY CELL DYE CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSFSERW: WEST BOUNDARY CELL SHELLFISH LARVAE TIME SERIES ID NUMBER\r\n" +
                    "*  NTXSERW: WEST BOUNDARY CELL TOXIC CONTAMINANT CONC TIME SERIES ID NUM.\r\n" +
                    "*  NSDSERW: WEST BOUNDARY CELL COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSNSERW: WEST BOUNDARY CELL NON-COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*\r\n" +
                    "C52    IBBW    JBBW  NTSCRW  NSSERW  NTSERW  NDSERW NSFSERW NTXSERW NSDSERW NSNSERW\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C53 TIME CONSTANT BOTTOM CONC ON WEST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING BOTTOM LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING BOTTOM LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING BOTTOM LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING BOTTOM LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING BOTTOM LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C53    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C54 TIME CONSTANT BOTTOM CONC ON WEST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING BOTTOM LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING BOTTOM LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C54   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C55 TIME CONSTANT SURFACE CONC ON WEST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING SURFAC LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING SURFAC LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING SURFAC LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING SURFAC LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING SURFAC LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C55    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C56 TIME CONSTANT SURFACE CONC ON WEST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING SURFAC LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING SURFAC LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C56   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C57 LOCATION OF CONC BC'S ON EAST BOUNDARIES AND SERIES IDENTIFIERS\r\n" +
                    "*\r\n" +
                    "*  ICBE:    I CELL INDEX\r\n" +
                    "*  JCBE:    J CELL INDEX\r\n" +
                    "*  NTSCRE:  NUMBER OF TIME STEPS TO RECOVER SPECIFIED VALUES ON CHANGE\r\n" +
                    "*           TO INFLOW FROM OUTFLOW\r\n" +
                    "*  NSSERE:  EAST BOUNDARY CELL SALINITY TIME SERIES ID NUMBER\r\n" +
                    "*  NTSERE:  EAST BOUNDARY CELL TEMPERATURE TIME SERIES ID NUMBER\r\n" +
                    "*  NDSERE:  EAST BOUNDARY CELL DYE CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSFSERE: EAST BOUNDARY CELL SHELLFISH LARVAE TIME SERIES ID NUMBER\r\n" +
                    "*  NTXSERE: EAST BOUNDARY CELL TOXIC CONTAMINANT CONC TIME SERIES ID NUM.\r\n" +
                    "*  NSDSERE: EAST BOUNDARY CELL COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSNSERE: EAST BOUNDARY CELL NON-COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*\r\n" +
                    "C57    IBBE    JBBE  NTSCRE  NSSERE  NTSERE  NDSERE NSFSERE NTXSERE NSDSERE NSNSERE\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C58 TIME CONSTANT BOTTOM CONC ON EAST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING BOTTOM LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING BOTTOM LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING BOTTOM LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING BOTTOM LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING BOTTOM LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C58    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C59 TIME CONSTANT BOTTOM CONC ON EAST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING BOTTOM LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING BOTTOM LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C59   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C60 TIME CONSTANT SURFACE CONC ON EAST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING SURFAC LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING SURFAC LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING SURFAC LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING SURFAC LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING SURFAC LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C60    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C61 TIME CONSTANT SURFACE CONC ON EAST CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING SURFAC LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING SURFAC LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C61   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C62 LOCATION OF CONC BC'S ON NORTH BOUNDARIES AND SERIES IDENTIFIERS\r\n" +
                    "*\r\n" +
                    "*  ICBN:    I CELL INDEX\r\n" +
                    "*  JCBN:    J CELL INDEX\r\n" +
                    "*  NTSCRN:  NUMBER OF TIME STEPS TO RECOVER SPECIFIED VALUES ON CHANGE\r\n" +
                    "*           TO INFLOW FROM OUTFLOW\r\n" +
                    "*  NSSERN:  NORTH BOUNDARY CELL SALINITY TIME SERIES ID NUMBER\r\n" +
                    "*  NTSERN:  NORTH BOUNDARY CELL TEMPERATURE TIME SERIES ID NUMBER\r\n" +
                    "*  NDSERN:  NORTH BOUNDARY CELL DYE CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSFSERN: NORTH BOUNDARY CELL SHELLFISH LARVAE TIME SERIES ID NUMBER\r\n" +
                    "*  NTXSERN: NORTH BOUNDARY CELL TOXIC CONTAMINANT CONC TIME SERIES ID NUM.\r\n" +
                    "*  NSDSERN: NORTH BOUNDARY CELL COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*  NSNSERN: NORTH BOUNDARY CELL NON-COHESIVE SED CONC TIME SERIES ID NUMBER\r\n" +
                    "*\r\n" +
                    "C62    IBBN    JBBN  NTSCRN  NSSERN  NTSERN  NDSERN NSFSERN NTXSERN NSDSERN NSNSERN\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C63 TIME CONSTANT BOTTOM CONC ON NORTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING BOTTOM LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING BOTTOM LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING BOTTOM LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING BOTTOM LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING BOTTOM LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C63    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C64 TIME CONSTANT BOTTOM CONC ON NORTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING BOTTOM LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING BOTTOM LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C64   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C65 TIME CONSTANT SURFACE CONC ON NORTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SAL: ULTIMATE INFLOWING SURFAC LAYER SALINITY\r\n" +
                    "*  TEM: ULTIMATE INFLOWING SURFAC LAYER TEMPERATURE\r\n" +
                    "*  DYE: ULTIMATE INFLOWING SURFAC LAYER DYE CONCENTRATION\r\n" +
                    "*  SFL: ULTIMATE INFLOWING SURFAC LAYER SHELLFISH LARVAE CONCENTRAION\r\n" +
                    "*  TOX: NTOX ULTIMATE INFLOWING SURFAC LAYER TOXIC CONTAMINANT\r\n" +
                    "*       CONCENTRATIONS NTOX VALUES TOX(N), N=1,NTOX\r\n" +
                    "*\r\n" +
                    "C65    SAL       TEM       DYE       SFL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C66 TIME CONSTANT SURFACE CONC ON NORTH CONC BOUNDARIES\r\n" +
                    "*\r\n" +
                    "*  SED: NSED ULTIMATE INFLOWING SURFAC LAYER COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRAIONS  FIRST NSED VALUES SED(N), N=1,NSND\r\n" +
                    "*  SND: NSND ULTIMATE INFLOWING SURFAC LAYER NON-COHESIVE SEDIMENT\r\n" +
                    "*       CONCENTRATIONS LAST NSND VALUES SND(N), N=1,NSND\r\n" +
                    "*\r\n" +
                    "C66   SED1    SND1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C66A CONCENTRATION DATA ASSIMILATION\r\n" +
                    "*\r\n" +
                    "*  NLCDA: NUMBER OF HORIZONTAL LOCATIONS FOR DATA ASSIMILATION\r\n" +
                    "*  TSCDA: WEIGHTING FACTOR, 0 to 1, 1 = FULL ASSIMILATION\r\n" +
                    "*  ISCDA: 1 FOR CONCENTRATION DATA ASSIMILATION VALUES (NC=1,7)\r\n" +
                    "*\r\n" +
                    "C66A  NLCDA   TSCDA   ISCDA\r\n" +
                    "          0       0       0       0       0       0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C66B CONCENTRATION DATA ASSIMILATION\r\n" +
                    "*\r\n" +
                    "*  ITPCDA: 0 ASSIMILATE DATA FROM TIME SERIES\r\n" +
                    "*          1 ASSIMIATED DATA FROM ANOTHER CELL IN GRID\r\n" +
                    "*    ICDA: I INDEX OF CELL ASSIMILATING DATA\r\n" +
                    "*    JCDA: J INDEX OF CELL ASSIMILATING DATA\r\n" +
                    "*   ICCDA: I INDEX OF CELL PROVIDING DATA, ITPCDA=1\r\n" +
                    "*   JCCDA: J INDEX OF CELL PROVIDING DATA, ITPCDA=1\r\n" +
                    "*  NCSERA:   ID OF TIME SERIES PROVIDING DATA\r\n" +
                    "*\r\n" +
                    "C66B ITPCDA    ICDA    JCDA   ICCDA   JCCDA      NS      NT      ND     NSF     NTX     NSD     NSN\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C67 DRIFTER DATA (FIRST 4 PARAMETER FOR SUB DRIFER, SECOND 6 FOR SUB LAGRES)\r\n" +
                    "*\r\n" +
                    "*  ISPD:   1 TO ACTIVE SIMULTANEOUS RELEASE AND LAGRANGIAN TRANSPORT OF\r\n" +
                    "*            NEUTRALLY BUOYANT PARTICLE DRIFTERS AT LOCATIONS INPUT ON C68\r\n" +
                    "*          2 TO ACTIVATE DS-INTERNATIONAL'S LPT DRIFTER COMPUTATIONS (DRIFTER.INP)\r\n" +
                    "*  NPD:      NUMBER OF PARTICLE DIRIFERS\r\n" +
                    "*  NPDRT:    TIME STEP AT WHICH PARTICLES ARE RELEASED\r\n" +
                    "*  NWPD:     NUMBER OF TIME STEPS BETWEEN WRITING TO TRACKING FILE\r\n" +
                    "*            DRIFTER.OUT\r\n" +
                    "*  ISLRPD: 1 TO ACTIVATE CALCULATION OF LAGRANGIAN MEAN VELOCITY OVER TIME\r\n" +
                    "*            INTERVAL TREF AND SPATIAL INTERVAL ILRPD1<I<ILRPD2,\r\n" +
                    "*            JLRPD1<J<JLRPD2, 1<K<KC, WITH MLRPDRT RELEASES.  ANY AVERGE\r\n" +
                    "*            OVER ALL RELEASE TIMES IS ALSO CALCULATED\r\n" +
                    "*          2 SAME BUT USES A HIGER ORDER TRAJECTORY INTEGRATION\r\n" +
                    "*  ILRPD1    WEST BOUNDARY OF REGION\r\n" +
                    "*  ILRPD2    EAST BOUNDARY OF REGION\r\n" +
                    "*  JLRPD1    NORTH BOUNDARY OF REGION\r\n" +
                    "*  JLRPD2    SOUTH BOUNDARY OF REGION\r\n" +
                    "*  MLRPDRT   NUMBER OF RELEASE TIMES\r\n" +
                    "*  IPLRPD    1,2,3 WRITE FILES TO PLOT ALL,EVEN,ODD HORIZ LAG VEL VECTORS\r\n" +
                    "*\r\n" +
                    "C67    ISPD     NPD   NPDRT    NWPD  ISLRPD  ILRPD1  ILRPD2  JLRPD1  JLRPD2 MLRPDRT  IPLRPD\r\n" +
                    "          0       0       0      12       0       3      28       9       7      12       1\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C68 INITIAL DRIFTER POSITIONS (FOR USE WITH SUB DRIFTER)\r\n" +
                    "*\r\n" +
                    "*  RI: I CELL INDEX IN WHICH PARTICLE IS RELEASED IN\r\n" +
                    "*  RJ: J CELL INDEX IN WHICH PARTICLE IS RELEASED IN\r\n" +
                    "*  RK: K CELL INDEX IN WHICH PARTICLE IS RELEASED IN\r\n" +
                    "*\r\n" +
                    "C68 RI RJ RK\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C69 CONSTANTS FOR CARTESION GRID CELL CENTER LONGITUDE AND LATITUDE\r\n" +
                    "*\r\n" +
                    "*  CDLON1: 6 CONSTANTS TO GIVE CELL CENTER LAT AND LON OR OTHER\r\n" +
                    "*  CDLON2: COORDINATES FOR CARTESIAN GRIDS USING THE FORMULAS\r\n" +
                    "*  CDLON3: DLON(L)=CDLON1+(CDLON2*FLOAT(I)+CDLON3)/60.\r\n" +
                    "*  CDLAT1: DLAT(L)=CDLAT1+(CDLAT2*FLOAT(J)+CDLAT3)/60.\r\n" +
                    "*  CDLAT2:\r\n" +
                    "*  CDLAT3:\r\n" +
                    "*\r\n" +
                    "C69 CDLON1 CDLON2 CDLON3 CDLAT1 CDLAT2 CDLAT3\r\n" +
                    "          0       0       0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C70 CONTROLS FOR WRITING ASCII OR BINARY DUMP FILES\r\n" +
                    "*\r\n" +
                    "*  ISDUMP: GREATER THAN 0 TO ACTIVATE\r\n" +
                    "*          1 SCALED ASCII INTERGER (0<VAL<65535)\r\n" +
                    "*          2 SCALED 16BIT BINARY INTEGER (0<VAL<65535) OR (-32768<VAL<32767)\r\n" +
                    "*          3 UNSCALED ASCII FLOATING POINT\r\n" +
                    "*          4 UNSCALED BINARY FLOATING POINT\r\n" +
                    "*  ISADMP: GREATER THAN 0 TO APPEND EXISTING DUMP FILES\r\n" +
                    "*  NSDUMP: NUMBER OF TIME STEPS BETWEEN DUMPS\r\n" +
                    "*  TSDUMP: STARTING TIME FOR DUMPS - DAYS (NO DUMPS BEFORE THIS TIME)\r\n" +
                    "*  TEDUMP: ENDING TIME FOR DUMPS - DAYS (NO DUMPS AFTER THIS TIME)\r\n" +
                    "*  ISDMPP: GREATER THAN 0 FOR WATER SURFACE ELEVATION DUMP\r\n" +
                    "*  ISDMPU: GREATER THAN 0 FOR HORIZONTAL VELOCITY DUMP\r\n" +
                    "*  ISDMPW: GREATER THAN 0 FOR VERTICAL VELOCITY DUMP\r\n" +
                    "*  ISDMPT: GREATER THAN 0 FOR TRANSPORTED VARIABLE DUMPS\r\n" +
                    "*  IADJDMP: 0 FOR SCALED BINARY INTEGERS (0<VAL<65535)\r\n" +
                    "*           -32768 FOR SCALED BINARY INTEGERS (-32768<VAL<32767)\r\n" +
                    "*\r\n" +
                    "C70  ISDUMP  ISADMP  NSDUMP  TSDUMP  TEDUMP  ISDMPP  ISDMPU  ISDMPW  ISDMPT IADJDMP\r\n" +
                    "          0       0       0       0       0       0       0       0       0  -32768\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C71 CONTROLS FOR HORIZONTAL PLANE SCALAR FIELD CONTOURING - RESIDUAL ONLY\r\n" +
                    "*\r\n" +
                    "*  ISSPH:    NOT USED\r\n" +
                    "*            \r\n" +
                    "*  NPSPH:    NOT USED\r\n" +
                    "*  ISRSPH: 1 TO WRITE FILE FOR RESIDUAL SCALAR VARIABLE IN HORIZONTAL PLANE\r\n" +
                    "*            \r\n" +
                    "*  ISPHXY: 0 DOES NOT WRITE I,J,X,Y IN ***CNH.OUT AND R***CNH.OUT FILES (RESIDUAL ONLY)\r\n" +
                    "*          1 WRITES I,J ONLY IN ***CNH.OUT AND R***CNH.OUT FILES (RESIDUAL ONLY)\r\n" +
                    "*          2 WRITES I,J,X,Y  IN ***CNH.OUT AND R***CNH.OUT FILES (RESIDUAL ONLY)\r\n" +
                    "*          \r\n" +
                    "*  DATA LINE REPEATS 7 TIMES FOR SAL,TEM,DYE,SFL,TOX,SED,SND\r\n" +
                    "*\r\n" +
                    "C71   ISSPH   NPSPH  ISRSPH  ISPHXY\r\n" +
                    "          0       1       0       3    !SAL\r\n" +
                    "          0       1       0       3    !TEM\r\n" +
                    "          0       1       0       3    !DYE\r\n" +
                    "          1       1       0       3    !SFL\r\n" +
                    "          0       1       0       3    !TOX\r\n" +
                    "          0       1       0       3    !SED\r\n" +
                    "          0       1       0       3    !SND\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C71A CONTROLS FOR HORIZONTAL PLANE SEDIMENT BED PROPERTIES CONTOURING\r\n" +
                    "* \r\n" +
                    "*  ISBPH:    NOT USED\r\n" +
                    "*\r\n" +
                    "* ISBEXP:  0 >0 EXPLORER BINARY FORMAT, OUTPUT FREQUENCY\r\n" +
                    "*  NPBPH:    NOT USED\r\n" +
                    "* ISRBPH:    NOT USED\r\n" +
                    "* ISBBDN:    NOT USED\r\n" +
                    "* ISBLAY:    NOT USED\r\n" +
                    "* ISBPOR:    NOT USED\r\n" +
                    "*  SBSED:    NOT USED\r\n" +
                    "*            \r\n" +
                    "*            \r\n" +
                    "* ISBSED:    NOT USED\r\n" +
                    "*            \r\n" +
                    "*            \r\n" +
                    "* ISBVDR:    NOT USED\r\n" +
                    "* ISBARD:    NOT USED\r\n" +
                    "*            \r\n" +
                    "*\r\n" +
                    "C71A  ISBPH  ISBEXP   NPBPH  ISRBPH  ISBBDN  ISBLAY  ISBPOR  ISBSED  ISBSND  ISBVDR  ISBARD\r\n" +
                    "          0       0       1       0       0       0       0       0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C71B FOOD CHAIN MODEL OUTPUT CONTROL\r\n" +
                    "* \r\n" +
                    "*     ISFDCH: 1 TO WRITE OUTPUT FOR HOUSATONIC RIVER FOOD CHAIN MODEL\r\n" +
                    "*     NFDCHZ:   NUMBER OF SPATIAL ZONES\r\n" +
                    "*     HBFDCH:   AVERAGING DEPTH FOR TOP PORTION OF BED (METERS)\r\n" +
                    "*     TFCAVG:   TIME AVERAGING INTERVAL FOR FOOD CHAIN OUTPUT (SECONDS)\r\n" +
                    "* \r\n" +
                    "C71B ISFDCH  NFDCHZ  HBFDCH  TFCAVG \r\n" +
                    "          0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C72 CONTROLS FOR EFDC_EXPLORER LINKAGE AND SURFACE ELEVATION RESIDUAL OUTPUT\r\n" +
                    "*\r\n" +
                    "*  ISPPH:  1 TO WRITE FILE FOR EFDC_EXPLORER LINKAGE\r\n" +
                    "*          2 WRITE ONLY FOR THE FIRST AND LAST REFERENCE TIME PERIOD\r\n" +
                    "*        100 TO ACTIVATE THE HIGH FREQUENCY DOMAIN OUTPUT READING SNAPSHOT.INP\r\n" +
                    "*  NPPPH:    NUMBER OF WRITES PER REFERENCE TIME PERIOD\r\n" +
                    "*  ISRPPH: 1 TO WRITE FILE FOR RESIDUAL SURFACE ELEVATION  CONTOURNG IN\r\n" +
                    "*            HORIZONTAL PLANE\r\n" +
                    "*  IPPHXY:   NOT USED\r\n" +
                    "*          \r\n" +
                    "*          \r\n" +
                    "*\r\n" +
                    "C72   ISPPH   NPPPH  ISRPPH  IPPHXY\r\n" +
                    "          1       1       0       3\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C73  CONTROLS FOR HORIZONTAL PLANE RESIDUAL VELOCITY VECTOR PLOTTING\r\n" +
                    "*\r\n" +
                    "*  ISVPH:    NOT USED\r\n" +
                    "*            NOT USED\r\n" +
                    "*  NPVPH:    NOT USED\r\n" +
                    "*  ISRVPH: 1 TO WRITE FILE FOR RESIDUAL VELOCITY PLOTTIN IN\r\n" +
                    "*            HORIZONTAL PLANE\r\n" +
                    "*  IVPHXY:   NOT USED\r\n" +
                    "*            NOT USED\r\n" +
                    "*            NOT USED\r\n" +
                    "*            NOT USED\r\n" +
                    "*\r\n" +
                    "C73   ISVPH   NPVPH  ISRVPH  IVPHXY\r\n" +
                    "          1       1       0       3\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C74  NOT USED\r\n" +
                    "*\r\n" +
                    "*  ISECSPV:  NOT USED\r\n" +
                    "*            \r\n" +
                    "*  NPSPV:    NOT USED\r\n" +
                    "*  ISSPV:    NOT USED\r\n" +
                    "*            \r\n" +
                    "*  ISRSPV:   NOT USED\r\n" +
                    "*  ISHPLTV:  NOT USED\r\n" +
                    "*  \r\n" +
                    "*  \r\n" +
                    "*  \r\n" +
                    "C74 ISECSPV   NPSPV   ISSPV  ISRSPV ISHPLTV\r\n" +
                    "          0       0       0       0       0    !SAL\r\n" +
                    "          0       0       0       0       0    !TEM\r\n" +
                    "          0       0       0       0       0    !DYE\r\n" +
                    "          0       0       0       0       0    !SFL\r\n" +
                    "          0       0       0       0       0    !TOX\r\n" +
                    "          0       0       0       0       0    !SED\r\n" +
                    "          0       0       0       0       0    !SND\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C75  NOT USED\r\n" +
                    "*\r\n" +
                    "*  ISECSPV: NOT USED\r\n" +
                    "*  NIJSPV:  NOT USED\r\n" +
                    "*  SEC ID:  NOT USED\r\n" +
                    "*\r\n" +
                    "C75 ISECSPV  NIJSPV     SEC ID\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C76  NOT USED\r\n" +
                    "*\r\n" +
                    "*  ISECSPV: NOT USED\r\n" +
                    "*  ISPV:    NOT USED\r\n" +
                    "*  JSPV:    NOT USED\r\n" +
                    "*\r\n" +
                    "C76 ISECSPV    ISPV    JSPV\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C77  NOT USED\r\n" +
                    "*\r\n" +
                    "*  ISECVPV: NOT USED\r\n" +
                    "*             \r\n" +
                    "*  NPVPV:   NOT USED\r\n" +
                    "*  ISVPV:   NOT USED\r\n" +
                    "*           \r\n" +
                    "*  ISRSPV:  NOT USED\r\n" +
                    "*\r\n" +
                    "C77 ISECVPV   NPVPV   ISVPV  ISRSPV\r\n" +
                    "          0       1       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C78  NOT USED\r\n" +
                    "*\r\n" +
                    "*  ISCEVPV: NOT USED\r\n" +
                    "*  NIJVPV:  NOT USED\r\n" +
                    "*  ANGVPV:  NOT USED\r\n" +
                    "*  SEC ID:  NOT USED\r\n" +
                    "*\r\n" +
                    "C78 ISECVPV  NIJVPV  ANGVPV     SEC     ID\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C79  NOT USED\r\n" +
                    "*\r\n" +
                    "*  ISECVPV: NOT USED\r\n" +
                    "*  IVPV:    NOT USED\r\n" +
                    "*  JVPV:    NOT USED\r\n" +
                    "*\r\n" +
                    "C79 ISECVPV    IVPV    JVPV\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C80 CONTROLS FOR 3D FIELD OUTPUT\r\n" +
                    "*\r\n" +
                    "*  IS3DO:  1 TO WRITE TO 3D ASCI INTEGER FORMAT FILES, JS3DVAR.LE.2     SEE|\r\n" +
                    "*          1 TO WRITE TO 3D ASCI FLOAT POINT FORMAT FILES, JS3DVAR.EQ.3 C57|\r\n" +
                    "*          2 TO WRITE TO 3D CHARACTER ARRAY FORMAT FILES (NOT ACTIVE)\r\n" +
                    "*          3 TO WRITE TO 3D HDF IMAGE FORMAT FILES (NOT ACTIVE)\r\n" +
                    "*          4 TO WRITE TO 3D HDF FLOATING POINT FORMAT FILES (NOT ACTIVE)\r\n" +
                    "*  ISR3DO:   SAME AS IS3DO EXCEPT FOR RESIDUAL VARIABLES\r\n" +
                    "*  NP3DO:    NUMBER OF WRITES PER LAST REF TIME PERIOD FOR INST VARIABLES\r\n" +
                    "*  KPC:      NUMBER OF UNSTRETCHED PHYSICAL VERTICAL LAYERS\r\n" +
                    "*  NWGG:     IF NWGG IS GREATER THAN ZERO, NWGG DEFINES THE NUMBER OF !2877|\r\n" +
                    "*            WATER CELLS IN CARTESIAN 3D GRAPHICS GRID OVERLAY OF THE\r\n" +
                    "*            CURVILINEAR GRID. FOR NWGG>0 AND EFDC RUNS ON A CURVILINEAR\r\n" +
                    "*            GRID, I3DMI,I3DMA,J3DMI,J3DMA REFER TO CELL INDICES ON THE\r\n" +
                    "*            ON THE CARTESIAN GRAPHICS GRID OVERLAY DEFINED BY FILE\r\n" +
                    "*            GCELL.INP.  THE FILE GCELL.INP IS NOT USED BY EFDC, BUT BY\r\n" +
                    "*            THE COMPANION GRID GENERATION CODE GEFDC.F.  INFORMATION\r\n" +
                    "*            DEFINING THE OVERLAY IS READ BY EFDC.F FROM THE FILE\r\n" +
                    "*            GCELLMP.INP. IF NWGG EQUALS 0, I3DMI,I3DMA,J3DMI,J3DMA REFER\r\n" +
                    "*            TO INDICES ON THE EFDC GRID DEFINED BY CELL.INP.\r\n" +
                    "*            ACTIVATION OF THE REWRITE OPTION I3DRW=1 WRITES TO THE FULL\r\n" +
                    "*            GRID DEFINED BY CELL.INP AS IF CELL.INP DEFINES A CARTESIAN\r\n" +
                    "*            GRID.  IF NWGG EQ 0 AND THE EFDC COMP GRID IS CO, THE REWRITE\r\n" +
                    "*            OPTION IS NOT RECOMMENDED AND A POST PROCESSOR SHOULD BE USED\r\n" +
                    "*            TO TRANSFER THE SHORT FORM, I3DRW=0, OUTPUT TO AN APPROPRIATE\r\n" +
                    "*            FORMAT FOR VISUALIZATION. CONTACT DEVELOPER FOR MORE DETAILS\r\n" +
                    "*  I3DMI:    MINIMUM OR BEGINNING I INDEX FOR 3D ARRAY OUTPUT\r\n" +
                    "*  I3DMA:    MAXIMUM OR ENDING I INDEX FOR 3D ARRAY OUTPUT\r\n" +
                    "*  J3DMI:    MINIMUM OR BEGINNING J INDEX FOR 3D ARRAY OUTPUT\r\n" +
                    "*  J3DMA:    MAXIMUM OR ENDING J INDEX FOR 3D ARRAY OUTPUT\r\n" +
                    "*  I3DRW:  0 FILES WRITTEN FOR ACTIVE CO WATER CELLS ONLY\r\n" +
                    "*          1 REWRITE FILES TO CORRECT ORIENTATION DEFINED BY GCELL.INP\r\n" +
                    "*            AND GCELLMP.INP FOR CO WITH NWGG.GT.O OR BY CELL.INP IF THE\r\n" +
                    "*            COMPUTATIONAL GRID IS CARTESIAN AND NWGG.EQ.0\r\n" +
                    "*  SELVMAX:  MAXIMUM SURFACE ELEVATION FOR UNSTRETCHING (ABOVE MAX SELV )\r\n" +
                    "*  BELVMIN:  MINIMUM BOTTOM ELEVATION FOR UNSTRETCHING (BELOW MIN BELV)\r\n" +
                    "*\r\n" +
                    "C80   IS3DO  ISR3DO   NP3DO     KPC    NWGG   I3DMI   I3DMA   J3DMI   J3DMA   I3DRW SELVMAX BELVMIN\r\n" +
                    "          0       0       6       1       0       1      29       1      14       0      15    -315\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C81 OUTPUT ACTIVATION AND SCALES FOR 3D FIELD OUTPUT\r\n" +
                    "*\r\n" +
                    "*  VARIABLE:     DUMMY VARIBLE ID (DO NOT CHANGE ORDER)\r\n" +
                    "*  IS3(VARID): 1 TO ACTIVATE THIS VARIBLES\r\n" +
                    "*  JS3(VARID): 0 FOR NO SCALING OF THIS VARIABLE\r\n" +
                    "*              1 FOR AUTO SCALING OF THIS VARIABLE OVER RANGE 0<VAL<255\r\n" +
                    "*                AUTO SCALES FOR EACH FRAME OUTPUT IN FILES OUT3D.DIA AND\r\n" +
                    "*                ROUT3D.DIA  OUTPUT IN I4 FORMAT\r\n" +
                    "*              2 FOR SCALING SPECIFIED IN NEXT TWO COLUMNS WITH OUTPUT\r\n" +
                    "*                DEFINED OVER RANGE  0<VAL<255 AND WRITTEN IN I4 FORMAT\r\n" +
                    "*              3 FOR MULTIPLIER SCALING BY MAX SCALE VALUE WITH OUTPUT\r\n" +
                    "*                WRITTEN IN F7.1 FORMAT (IS3DO AND ISR3DO MUST BE 1)\r\n" +
                    "*\r\n" +
                    "C81 VARIABLE   IS3D    JS3D    SMAX    SMIN\r\n" +
                    "    'U VEL'       0       0       0       0\r\n" +
                    "    'V VEL'       0       0       0       0\r\n" +
                    "    'W VEL'       0       0       0       0\r\n" +
                    "      'SAL'       0       0       0       0\r\n" +
                    "     'TEMP'       0       0       0       0\r\n" +
                    "      'DYE'       0       0       0       0\r\n" +
                    "      'SED'       0       0       0       0\r\n" +
                    "      'SND'       0       0       0       0\r\n" +
                    "      'TOX'       0       0       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C82 INPLACE HARMONIC ANALYSIS PARAMETERS\r\n" +
                    "*\r\n" +
                    "*  ISLSHA:  1 FOR IN PLACE LEAST SQUARES HARMONIC ANALYSIS\r\n" +
                    "*  MLLSHA:  NUMBER OF LOCATIONS FOR LSHA\r\n" +
                    "*  NTCLSHA: LENGTH OF LSHA IN INTEGER NUMBER OF REFERENCE TIME PERIODS\r\n" +
                    "*  ISLSTR:  1 FOR TREND REMOVAL\r\n" +
                    "*  ISHTA :  1 FOR SINGLE TREF PERIOD SURFACE ELEV ANALYSIS\r\n" +
                    "*                    90\r\n" +
                    "C82  ISLSHA  MLLSHA NTCLSHA  ISLSTR   ISHTA\r\n" +
                    "          0       0       2       0       0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C83 HARMONIC ANALYSIS LOCATIONS AND SWITCHES\r\n" +
                    "*\r\n" +
                    "*  ILLSHA:   I CELL INDEX\r\n" +
                    "*  JLLSHA:   J CELL INDEX\r\n" +
                    "*  LSHAP:  1 FOR ANALYSIS OF SURFACE ELEVATION\r\n" +
                    "*  LSHAB:  1 FOR ANALYSIS OF SALINITY\r\n" +
                    "*  LSHAUE: 1 FOR ANALYSIS OF EXTERNAL MODE HORIZONTAL VELOCITY\r\n" +
                    "*  LSHAU:  1 FOR ANALYSIS OF HORIZONTAL VELOCITY IN EVERY LAYER\r\n" +
                    "*  CLSL:     LOCATION AS A CHARACTER VARIALBLE\r\n" +
                    "*\r\n" +
                    "C83  ILLSHA  JLLSHA   LSHAP   LSHAB  LSHAUE   LSHAU    CLSL\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C84 CONTROLS FOR WRITING TO TIME SERIES FILES\r\n" +
                    "*\r\n" +
                    "*  ISTMSR:  1 OR 2 TO WRITE TIME SERIES OF SURF ELEV, VELOCITY, NET\r\n" +
                    "*           INTERNAL AND EXTERNAL MODE VOLUME SOURCE-SINKS, AND\r\n" +
                    "*           CONCENTRATION VARIABLES,  2 APPENDS EXISTING TIME SERIES FILES\r\n" +
                    "*  MLTMSR:  NUMBER HORIZONTAL LOCATIONS TO WRITE TIME SERIES OF SURF ELEV,\r\n" +
                    "*           VELOCITY, AND CONCENTRATION VARIABLES\r\n" +
                    "*  NBTMSR:  TIME STEP TO BEGIN WRITING TO TIME SERIES FILES (Inactive)\r\n" +
                    "*  NSTMSR:  TIME STEP TO STOP WRITING TO TIME SERIES FILES (Inactive)\r\n" +
                    "*  NWTMSR:  NUMBER OF TIME STEPS TO SKIP BETWEEN OUTPUT\r\n" +
                    "*  NTSSTSP: NUMBER OF TIME SERIES START-STOP SCENARIOS,  1 OR GREATER\r\n" +
                    "*  TCTMSR:  UNIT CONVERSION FOR TIME SERIES TIME.  FOR SECONDS, MINUTES,\r\n" +
                    "*           HOURS,DAYS USE 1.0, 60.0, 3600.0, 86400.0 RESPECTIVELY\r\n" +
                    "*\r\n" +
                    "*\r\n" +
                    "C84  ISTMSR  MLTMSR  NBTMSR  NSTMSR  NWTMSR NTSSTSP  TCTMSR\r\n" +
                    "          0       0       0       0       1       0   86400\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C85 CONTROLS FOR WRITING TO TIME SERIES FILES\r\n" +
                    "*\r\n" +
                    "*  ITSSS:   START-STOP SCENARIO NUMBER 1.GE.ISSS.LE.NTSSTSP\r\n" +
                    "*  MTSSTSP: NUMBER OF STOP-START PAIRS FOR SCENARIO ISSS\r\n" +
                    "*\r\n" +
                    "C85 ITSSS MTSSTSP\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C86 CONTROLS FOR WRITING TO TIME SERIES FILES\r\n" +
                    "*\r\n" +
                    "*  ITSSS:  START-STOP SCENARIO NUMBER 1.GE.ISSS.LE.NTSSTSP\r\n" +
                    "*  MTSSS:  NUMBER OF STOP-START PAIRS FOR SCENARIO ISSS\r\n" +
                    "*  TSSTRT: STARTING TIME FOR SCENARIO ITSSS, SAVE INTERVAL MTSSS\r\n" +
                    "*  TSSTOP: STOPING TIME FOR SCENARIO ITSSS, SAVE INTERVAL MTSSS\r\n" +
                    "*                   -1000.\r\n" +
                    "C86    ISSS   MTSSS  TSSTRT  TSSTOP   COMMENT\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C87 CONTROLS FOR WRITING TO TIME SERIES FILES\r\n" +
                    "*\r\n" +
                    "*  ILTS:    I CELL INDEX\r\n" +
                    "*  JLTS:    J CELL INDEX\r\n" +
                    "*  NTSSSS:  WRITE SCENARIO FOR THIS LOCATION\r\n" +
                    "*  MTSP:  1 FOR TIME SERIES OF SURFACE ELEVATION\r\n" +
                    "*  MTSC:  1 FOR TIME SERIES OF TRANSPORTED CONCENTRATION VARIABLES\r\n" +
                    "*  MTSA:  1 FOR TIME SERIES OF EDDY VISCOSITY AND DIFFUSIVITY\r\n" +
                    "*  MTSUE: 1 FOR TIME SERIES OF EXTERNAL MODE HORIZONTAL VELOCITY\r\n" +
                    "*  MTSUT: 1 FOR TIME SERIES OF EXTERNAL MODE HORIZONTAL TRANSPORT\r\n" +
                    "*  MTSU:  1 FOR TIME SERIES OF HORIZONTAL VELOCITY IN EVERY LAYER\r\n" +
                    "*  MTSQE: 1 FOR TIME SERIES OF NET EXTERNAL MODE VOLUME SOURCE/SINK\r\n" +
                    "*  MTSQ:  1 FOR TIME SERIES OF NET EXTERNAL MODE VOLUME SOURCE/SINK\r\n" +
                    "*  CLTS:    LOCATION AS A CHARACTER VARIALBLE\r\n" +
                    "*\r\n" +
                    "C87    ILTS    JLTS  NTSSSS    MTSP    MTSC    MTSA   MTSUE   MTSUT    MTSU   MTSQE    MTSQ    CLTS\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C88  High frequency output for specific locations and times\r\n" +
                    "*\r\n" +
                    "* HFREOUT:  1 use high frequency dates for output\r\n" +
                    "*           0 specific output option is not used\r\n" +
                    "*\r\n" +
                    "C88 HFREOUT\r\n" +
                    "          0\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C89 NOT USED\r\n" +
                    "*\r\n" +
                    "*  MMDVSFP:  NOT USED\r\n" +
                    "*  DMSFP:    NOT USED\r\n" +
                    "*\r\n" +
                    "C89 MMDVSFP  DMVSFP\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C90 NOT USED\r\n" +
                    "*\r\n" +
                    "*  MMLVSFP: NOT USED\r\n" +
                    "*  TIMVSFP: NOT USED\r\n" +
                    "*  IVSFP:   NOT USED\r\n" +
                    "*  JVSFP:   NOT USED\r\n" +
                    "*\r\n" +
                    "C90 MMLVSFP TIMVSFP   IVSFP   JVSFP\r\n" +
                    "-------------------------------------------------------------------------------\r\n" +
                    "C91 OPTIONS FOR GENERATION OF NETCDF FILE(S)\r\n" +
                    "*\r\n" +
                    "*  NCDFOUT:         OPTION FOR NETCDF EXPORT\r\n" +
                    "*               =1  GENERATE NETCDF FILE NC\r\n" +
                    "*               =0  NO GENERATION\r\n" +
                    "*  DEFLEV:          LEVEL OF COMPRESSION OF NETCDF FILE FROM 0 TO 9\r\n" +
                    "*  ROTA:        =1  ROTATING 2D VELOCITY FIELD TO THE TRUE EAST AND TRUE NORTH\r\n" +
                    "*               =0  NO ROTATION TO TRUE EAST AND TRUE NORTH\r\n" +
                    "*  BLK:             MISSING VALUE: INTEGER KIND\r\n" +
                    "*  UTMZ:            UTM ZONE\r\n" +
                    "*                   >0 FOR NORTHERN HEMISPHERE; <0 FOR SOUTHERN HEMISPHERE\r\n" +
                    "*  HREST:           NUMBER OF HOURS AFTER BEGIN TIME FOR RESTART OUTPUT\r\n" +
                    "*  BASEDATE:        YYYY-MM-DD (NO BLANK)\r\n" +
                    "*  BASETIME:        HH:MM:SS   (NO BLANK)\r\n" +
                    "*  PROJ:            PROJECT NAME IS A STRING OF MAXIMUM LENGTH 20\r\n" +
                    "*                   WITHOUT ANY BLANKS\r\n" +
                    "*\r\n" +
                    "C91 NCDFOUT  DEFLEV    ROTA     BLK    UTMZ   HREST     BASEDATE    BASETIME    PROJ\r\n" +
                    "          0       2       1    -999      17    24.0   2004-01-01    00:00:00   EFDC");
        } catch (Exception e) {
            success = false;
            e.printStackTrace();
            throw e;
        } finally {
            writer.close();
        }
        return success;
    }
        @Override
    public boolean writeAserFile(String planId, String path) throws Exception {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            String[] items = new String[]{"aser_01", "aser_02", "aser_03", "aser_04", "aser_05", "aser_06", "aser_07", "aser_08", "aser_09", "aser_10"};
            Map<String, String> map = dao.getInputItemData(planId, items);
            List<NameValuePair> taiyangfusheList = dao.getFileItemData(planId, "73920C3DAF3B4317935CAA5FF1DD46A8").get(0);
            List<NameValuePair> qiyaList = dao.getFileItemData(planId, "D19CCCB4732F46859FB6E2B0A58F3174").get(0);
            List<NameValuePair> qiwenList = dao.getFileItemData(planId, "8B15A500C6174F078EDCE9B4DE137B4A").get(0);
            List<NameValuePair> xiangduishiduList = dao.getFileItemData(planId, "6926DAEBC89B4D71883CD22A0A651C77").get(0);
            List<NameValuePair> jiangyuList = dao.getFileItemData(planId, "2A4201E041EF4EDEA66DEAAA671466E2").get(0);
            List<NameValuePair> zhengfaliangList = dao.getFileItemData(planId, "C15F02D85FA34A44A9F1EFF58D5898B2").get(0);
            List<NameValuePair> yuncengList = dao.getFileItemData(planId, "7F11E44802404EC0AF4DF1C6AC6D7CBF").get(0);
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            int length = taiyangfusheList.size();
            if (qiyaList.size() < length) {
                length = qiyaList.size();
            }
            if (qiwenList.size() < length) {
                length = qiwenList.size();
            }
            if (xiangduishiduList.size() < length) {
                length = xiangduishiduList.size();
            }
            if (jiangyuList.size() < length) {
                length = jiangyuList.size();
            }
            if (zhengfaliangList.size() < length) {
                length = zhengfaliangList.size();
            }
            if (yuncengList.size() < length) {
                length = yuncengList.size();
            }
            writer.write("C ** , aser.inp Time Series FILE\r\n" +
                    "C **\r\n" +
                    "C **  ATMOSPHERIC FORCING FILE, USE WITH 28 JULY 96 AND LATER VERSIONS OF EFDC\r\n" +
                    "C **\r\n" +
                    "C **  MASER     =NUMBER OF TIME DATA POINTS\r\n" +
                    "C **  TCASER    =DATA TIME UNIT CONVERSION TO SECONDS\r\n" +
                    "C **  TAASER    =ADDITIVE ADJUSTMENT OF TIME VALUES SAME UNITS AS INPUT TIMES\r\n" +
                    "C **  IRELH     =0 VALUE TWET COLUMN VALUE IS TWET, =1 VALUE IS RELATIVE HUMIDITY\r\n" +
                    "C **  RAINCVT   =CONVERTS RAIN TO UNITS OF M/SEC , inch/day=0.0254m/86400s=2.94E-7m/s, inch/h=7.05556E-6m/s\r\n" +
                    "C **  EVAPCVT   =CONVERTS EVAP TO UNITS OF M/SEC, IF EVAPCVT<0 EVAP IS INTERNALLY COMPUTED\r\n" +
                    "C **  SOLRCVT   =CONVERTS SOLAR SW RADIATION TO JOULES/S/SQ METER (Watts/m^2)\r\n" +
                    "C **  CLDCVT    =MULTIPLIER FOR ADJUSTING CLOUD COVER\r\n" +
                    "C **  IASWRAD   =O DISTRIBUTE SW SOL RAD OVER WATER COL AND INTO BED, =1 ALL TO SURF LAYER\r\n" +
                    "C **  REVC      =1000*EVAPORATIVE TRANSFER COEF, REVC<0 USE WIND SPD DEPD DRAG COEF\r\n" +
                    "C **  RCHC      =1000*CONVECTIVE HEAT TRANSFER COEF, REVC<0 USE WIND SPD DEPD DRAG COEF\r\n" +
                    "C **  SWRATNF   =FAST SCALE SOLAR SW RADIATION ATTENUATION COEFFCIENT 1./METERS\r\n" +
                    "C **  SWRATNS   =SLOW SCALE SOLAR SW RADIATION ATTENUATION COEFFCIENT 1./METERS\r\n" +
                    "C **  FSWRATF   =FRACTION OF SOLSR SW RADIATION ATTENUATED FAST  0<FSWRATF<1\r\n" +
                    "C **  DABEDT    =DEPTH OR THICKNESS OF ACTIVE BED TEMPERATURE LAYER, METERS\r\n" +
                    "C **  TBEDIT    =INITIAL BED TEMPERATURE\r\n" +
                    "C **  HTBED1    =CONVECTIVE HT COEFFCIENT BETWEEN BED AND BOTTOM WATER LAYER  NO DIM\r\n" +
                    "C **  HTBED2    =HEAT TRANS COEFFCIENT BETWEEN BED AND BOTTOM WATER LAYER  M/SEC\r\n" +
                    "C **  PATM      =ATM PRESS (MILLIBAR)\r\n" +
                    "C **  TDRY/TEQ  =DRY ATM TEMP ISOPT(2)=1 OR EQUIL TEMP ISOPT(2)=2 (DEG C)\r\n" +
                    "C **  TWET/RELH =WET BULB ATM TEMP IRELH=0, RELATIVE HUMIDITY IRELH=1 (DEG C/DIMENSIONLESS)\r\n" +
                    "C **  RAIN      =RAIN FALL RATE LENGTH/TIME       (EE UNITS - M/DAY)\r\n" +
                    "C **  EVAP      =EVAPORATION RATE IS EVAPCVT>0.   (EE UNITS - M/DAY)\r\n" +
                    "C **  SOLSWR    =SOLAR SHORT WAVE RADIATION AT WATER SURFACE (WATTS/M2) \r\n" +
                    "C **  CLOUD     =FRACTIONAL CLOUD COVER  (DIMENSIONLESS)\r\n" +
                    "EE    EFDC_DS_SOLRAD (COMPUTE SOLAR RADIATION FROM LAT/LONG & CLOUD COVER): False\r\n" +
                    "EE    EFDC_DS_LATITUDE  (DEC DEGREES):40.037\r\n" +
                    "EE    EFDC_DS_LONGITUDE (DEC DEGREES):117.531\r\n" +
                    "EE    EFDC_DS_USESHADE  (T/F): False\r\n" +
                    "C **   MASER    TCASER    TAASER     IRELH     RAINCVT    EVAPCVT    SOLRCVT    CLDCVT        X(M)        Y(M)\r\n" +
                    "C **  IASWRAD      REVC      RCHC   SWRATNF   SWRATNS   FSWRATF    DABEDT    TBEDIT    HTBED1    HTBED2\r\n" +
                    "C ** TASER(D)  PATM(MB)   TDRY(C)   TWET(C) RAIN(M/D) EVAP(M/D) SOLSWR(W/M2)  CLOUD\r\n");

            writer.write("       " + qiyaList.size() + " 86400.000     0.000         0  1.1574E-05     -1          1.000     1.000   ! Yuqiao Atm\r\n");
            writer.write("            " + map.get("aser_01") + "        " + map.get("aser_02") + "        " + map.get("aser_03") + "        " + map.get("aser_04") + "        " + map.get("aser_05") + "        " + map.get("aser_06") + "        " + map.get("aser_07") + "        " + map.get("aser_08") + "        " + map.get("aser_09") + "        " + map.get("aser_10") + "\r\n");
            for (int i = 0; i < length; i++) {
                writer.write("     " + qiyaList.get(i).getName() + " " + qiyaList.get(i).getValue()
                        + "   " + qiwenList.get(i).getValue() + "    " + xiangduishiduList.get(i).getValue()
                        + "    " + jiangyuList.get(i).getValue() + "    " + zhengfaliangList.get(i).getValue()
                        + "   " + taiyangfusheList.get(i).getValue() + "    " + yuncengList.get(i).getValue() + "\r\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }

        return success;
    }
        @Override
    public boolean wirteDserFile(String planId, String path) throws IOException, SQLException {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            List<List<NameValuePair>> wuranwuList = dao.getFileItemData(planId, "EA2D29994A254C1B85C5B0C8F53A8648");
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("C ** , dser.inp Time Series FILE\r\n" +
                    "C ** REPEATS NCSER(3) TIMES\r\n" +
                    "C ** \r\n" +
                    "C ** InType  MCSER(NS)  TCCSER(NS)  TACSER(NS)  RMULADJ(NS)  ADDADJ(NS)\r\n" +
                    "C ** \r\n" +
                    "C **  IF InType.EQ.1 THEN READ DEPTH WEIGHTS AND SINGLE VALUE OF Dye\r\n" +
                    "C **                      ELSE READ A VALUE OF DYE FOR EACH LAYER\r\n" +
                    "C **  \r\n" +
                    "C **  InType=1 Structure\r\n" +
                    "C **  WKQ(K),K=1,KC\r\n" +
                    "C **  TCSER(M,NS)    CSER(M,1,NS)          !(MCSER(NS,3) PAIRS FOR NS=1,NCSER(3) SERIES)\r\n" +
                    "C **\r\n" +
                    "C **  InType=0 Structure\r\n" +
                    "C **  TCSER(M,NS)    (CSER(M,K,NS),K=1,KC) !(MCSER(NS) PAIRS)\r\n" +
                    "C ** \r\n");
            for (int i = 0; i < wuranwuList.size(); i++) {
                writer.write("          0       7   86400       0       1       0 !  dye flow in_1\r\n");
                for (int j = 0; j < wuranwuList.get(i).size(); j++) {
                    writer.write("     " + wuranwuList.get(i).get(j).getName() + "    " + wuranwuList.get(i).get(j).getValue() + "\r\n");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }

        return success;
    }
        @Override
    public boolean wirteQserFile(String planId, String path) throws IOException, SQLException {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            List<List<NameValuePair>> wuranwuList = dao.getFileItemData(planId, "24D8C75DC4024CA78213956FE33F01BC");
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("C ** , qser.inp Time Series FILE\r\n" +
                    "C **\r\n" +
                    "C **  InType MQSER(NS) TCQSER(NS) TAQSER(NS) RMULADJ(NS) ADDADJ(NS) ICHGQS\r\n" +
                    "C **\r\n" +
                    "C **  IF InType.EQ.1 THEN READ DEPTH WEIGHTS AND SINGLE VALUE OF QSER\r\n" +
                    "C **                      ELSE READ A VALUE OF QSER FOR EACH LAYER\r\n" +
                    "C **  \r\n" +
                    "C **  InType=1 Structure\r\n" +
                    "C **  WKQ(K),K=1,KC\r\n" +
                    "C **  TQSER(M,NS)    QSER(M,1,NS)          !(MQSER(NS) PAIRS FOR NS=1,NQSER SERIES)\r\n" +
                    "C **\r\n" +
                    "C **  InType=0 Structure\r\n" +
                    "C **  TQSER(M,NS)    (QSER(M,K,NS),K=1,KC) !(MQSER(NS) PAIRS)\r\n" +
                    "C **\r\n");
            for (int i = 0; i < wuranwuList.size(); i++) {
                writer.write("          0     366   86400       0       1       0       0 !  ShaLi_Flow in\r\n");
                for (int j = 0; j < wuranwuList.get(i).size(); j++) {
                    writer.write("     " + wuranwuList.get(i).get(j).getName() + "    " + wuranwuList.get(i).get(j).getValue() + "\r\n");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }

        return success;
    }
        @Override
    public boolean writeTserFile(String planId, String path) throws IOException, SQLException {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            List<List<NameValuePair>> fengchangList = dao.getFileItemData(planId, "343446D90C3144ABA493952C9975B216");
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("C ** , tser.inp Time Series FILE\r\n" +
                    "C ** REPEATS NCSER(2) TIMES\r\n" +
                    "C ** \r\n" +
                    "C ** InType  MCSER(NS)  TCCSER(NS)  TACSER(NS)  RMULADJ(NS)  ADDADJ(NS)\r\n" +
                    "C ** \r\n" +
                    "C **  IF InType.EQ.1 THEN READ DEPTH WEIGHTS AND SINGLE VALUE OF Temp\r\n" +
                    "C **                      ELSE READ A VALUE OF TEMP FOR EACH LAYER\r\n" +
                    "C **  \r\n" +
                    "C **  InType=1 Structure\r\n" +
                    "C **  WKQ(K),K=1,KC\r\n" +
                    "C **  TCSER(M,NS)    CSER(M,1,NS)          !(MCSER(NS,2) PAIRS FOR NS=1,NCSER(2) SERIES)\r\n" +
                    "C **\r\n" +
                    "C **  InType=0 Structure\r\n" +
                    "C **  TCSER(M,NS)    (CSER(M,K,NS),K=1,KC) !(MCSER(NS) PAIRS)\r\n" +
                    "C ** \r\n");
            for (int i = 0; i < fengchangList.size(); i++) {
                writer.write("          0      25   86400       0       1       0 !  Guoheqiao Temperature\r\n");
                for (int j = 0; j < fengchangList.get(i).size(); j++) {
                    writer.write("     " + fengchangList.get(i).get(j).getName() + "    " + fengchangList.get(i).get(j).getValue() + "\r\n");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }

        return success;
    }
        @Override
    public boolean writeTempFile(String planId, String path) throws IOException, SQLException {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            String[] params = new String[]{"temp_dafault"};
            Map<String, String> map = dao.getInputItemData(planId, params);
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("C  , temp.inp IC FILE\r\n" +
                    "C  ***         ISTYP 0 - ParamVal(1,KC)\r\n" +
                    "C  ***               1 - L, I, J, ParamVal(1,KC)\r\n" +
                    "C  ******************************************************************************\r\n" +
                    " 0\r\n");
            String value = map.get("temp_dafault");
            for (int i = 0; i < 1926; i++) {
                writer.write(value + "\r\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }

        return success;
    }
        @Override
    public boolean writeWserFile(String planId, String path) throws IOException, SQLException {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            List<NameValuePair> fengsuList = dao.getFileItemData(planId, "E5F089CDE5504F3789F701B840A403B7").get(0);
            List<NameValuePair> fengxiangList = dao.getFileItemData(planId, "6A9F342796A940AC847A0BC843EB01F4").get(0);
                int length = fengsuList.size();
                if(fengxiangList.size()<length){length =fengxiangList.size();}
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("C ** , wser.inp Time Series FILE\r\n" +
                    "C **  WIND FORCING FILE, USE WITH 7 APRIL 97 AND LATER VERSIONS OF EFDC\r\n" +
                    "C **\r\n" +
                    "C **  MASER(NW)     =NUMBER OF TIME DATA POINTS\r\n" +
                    "C **  TCASER(NW)    =DATA TIME UNIT CONVERSION TO SECONDS\r\n" +
                    "C **  TAASER(NW)    =ADDITIVE ADJUSTMENT OF TIME VALUES SAME UNITS AS INPUT TIMES\r\n" +
                    "C **  WINDSCT(NW)   =WIND SPEED CONVERSION TO M/SEC\r\n" +
                    "C **  ISWDINT(NW)   =DIRECTION CONVENTION\r\n" +
                    "C **               0 DIRECTION TOWARDS\r\n" +
                    "C **               1 DIRECTION FROM\r\n" +
                    "C **               2 WINDS IS EAST VELOCITY, WINDD IS NORTH VELOCITY\r\n" +
                    "C **\r\n" +
                    "EE    EFDC_DS_WIND_HEIGHT (m):10.0\r\n" +
                    "EE    EFDC_DS_ICE_PERIOD_START (days):0.000\r\n" +
                    "EE    EFDC_DS_ICE_PERIOD_END   (days):0.000\r\n" +
                    "C **  MASER    TCASER    TAASER   WINDSCT   ISWDINT\r\n" +
                    "C **  TASER(M) WINDS(M/S) WINDD(DEG)\r\n" +
                    "         13   86400       0       1       0 ! Wind station\r\n" +
                    "          0       7   86400       0       1       0 !  dye flow in_1\n");
            for (int j = 0; j < length; j++) {
                writer.write("     " + fengsuList.get(j).getName() + "    " + fengsuList.get(j).getValue() + "   "+fengxiangList.get(j).getValue()+"\r\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }

        return success;
    }
        @Override
    public boolean writeDyeFile(String planId, String path) throws IOException, SQLException {
        boolean success = true;
        BufferedWriter writer = null;
        try {
            String[] params = new String[]{"dye_dafault"};
            Map<String, String> map = dao.getInputItemData(planId, params);
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path)));
            writer.write("C  , dye.inp IC FILE\r\n" +
                    "C  ***         ISTYP 0 - ParamVal(1,KC)\r\n" +
                    "C  ***               1 - L, I, J, ParamVal(1,KC)\r\n" +
                    "C  ******************************************************************************\r\n" +
                    " 0\r\n");
            String value = map.get("dye_dafault");
            for (int i = 0; i < 1926; i++) {
                writer.write(value + "\r\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }
        return success;
    }
}
