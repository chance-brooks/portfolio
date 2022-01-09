package hotciv.standard;

import hotciv.framework.*;

public class SemiCivFactory implements GameTypeFactory {
    /*
    SemiCiv implements the following...
    BetaCiv world aging strategy
    DeltaCiv world layout strategy
    GammaCiv settler archer strategy (I.E. unit actions)
    EpsilonCiv winner strategy
    EpsilonCiv attacking strategy
    City and workforce ->>> Alpha unless EtaCiv is implemented
        Alpha will be used for now. A City and Workforce strategy does not exist currently.
    */
    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.BETACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.GAMMACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.EPSILONCIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new DeltaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){
        return new AttackStrategyImpl(GameConstants.EPSILONCIV);
    }

}
