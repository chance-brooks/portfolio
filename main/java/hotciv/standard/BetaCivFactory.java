package hotciv.standard;

import hotciv.framework.*;

public class BetaCivFactory implements GameTypeFactory{
    //BetaCiv differs in the Win Strategy and the Aging Strategy
    //SettlerArcher Strategy and World layout strategy are the same as AlphaCiv

    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.BETACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.ALPHACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.BETACIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new AlphaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){ return new AttackStrategyImpl(GameConstants.ALPHACIV); }

}
