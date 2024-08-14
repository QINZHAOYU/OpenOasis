import os
import unittest

from DataAsst.MeshConverters.GmshMesh import FromGmsh, ToGmsh


class TestGmshMesh(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        print(f"\n---------- Testing < {cls.__name__} > \n")

    @classmethod
    def tearDownClass(cls):
        print("\n---------- Done \n")

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_mesh_from_gmsh(self):
        msh_file = r""
        mesh = FromGmsh(msh_file)
        points = mesh.read_nodes()
        cells = mesh.read_cells()
        print(f"points: {points}")
        print(f"cells: {cells}")

    def test_mesh_to_oasis(self):
        pass


if __name__ == "__main__":
    # 项目根目录下测试
    with open("./DataAsst/tests/reports/reports.txt", "w", encoding="utf8") as reporter:
        # 方式一：一次加载整个测试类
        # suit = unittest.makeSuite(TestGmshMesh)

        # 方式二：逐次加载测试用例
        suit = unittest.TestSuite()
        suit.addTest(TestGmshMesh("test_mesh_from_gmsh"))

        # 执行测试
        runner = unittest.TextTestRunner(stream=reporter, verbosity=2)
        runner.run(suit)
