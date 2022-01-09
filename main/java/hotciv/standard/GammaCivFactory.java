package hotciv.standard;

import hotciv.framework.*;

public class GammaCivFactory implements GameTypeFactory {
    //GammaCiv differs in the SettlerArcher Strategy
    //All else is the same as AlphaCiv

    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.ALPHACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.GAMMACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.ALPHACIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new AlphaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){ return new AttackStrategyImpl(GameConstants.ALPHACIV); }

}
