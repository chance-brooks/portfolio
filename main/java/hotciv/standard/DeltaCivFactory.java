package hotciv.standard;

import hotciv.framework.*;

public class DeltaCivFactory implements GameTypeFactory {
    //DeltaCiv differs in World Layout
    // All else is the same as AlphaCiv

    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.ALPHACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.ALPHACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.ALPHACIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new DeltaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){ return new AttackStrategyImpl(GameConstants.ALPHACIV); }

}
