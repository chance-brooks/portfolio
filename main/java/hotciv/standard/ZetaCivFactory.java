package hotciv.standard;

import hotciv.framework.*;

public class ZetaCivFactory implements GameTypeFactory {
    //Win strategy differs. All else are the same as AlphaCiv
    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.ALPHACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.ALPHACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.ZETACIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new AlphaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){ return new AttackStrategyImpl(GameConstants.ALPHACIV); }

}
