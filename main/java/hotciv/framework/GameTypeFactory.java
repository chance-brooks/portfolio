package hotciv.framework;

public interface GameTypeFactory {
    public AgeStrategy createAgeStrategy();

    public UnitActionStrategy createUnitActionStrategy();

    public WinStrategy createWinStrategy();

    public WorldLayoutStrategy createWorldLayoutStrategy();

    public AttackStrategy createAttackStrategy();

}
