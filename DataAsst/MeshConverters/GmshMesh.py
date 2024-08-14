# -*- encoding: utf-8 -*-
"""
针对 Gmsh4 的 MESH 格式网格文件解析器, 用于 Gmsh4 和 OpenOasis 之间的网格转换。
"""
import meshio
import os


class FromGmsh:
    """
    从 Gmsh4 的 MESH 格式网格文件解析节点、面、单元信息。
    """

    def __init__(self, gmsh_file: str):
        self.gmsh_file = gmsh_file
        self.mesh = meshio.read(gmsh_file)

    def read_nodes(self):
        """
        读取节点信息。
        """
        return self.mesh.points

    def read_faces(self):
        """
        读取面信息。
        """
        return None

    def read_cells(self):
        """
        读取单元信息。
        """
        return self.mesh.cells

    def to_vtk(self, output_dir: str = "."):
        """
        将 Gmsh4 的 MESH 格式网格文件转换为 VTK 格式。

        Parameters:
            output_dir: 输出目录。
        """
        file = os.path.basename(self.gmsh_file)
        name, _ = os.path.splitext(file)

        # 输出 VTK 文件
        vtk_file = os.path.join(output_dir, name + ".vtk")
        meshio.write(vtk_file, self.mesh, file_format="vtk")

    def to_oasis(self, output_dir: str = "."):
        """
        将 Gmsh4 的 MESH 格式网格文件转换为 OpenOasis 的网格格式。

        Parameters:
            output_dir: 输出目录。
        """
        file = os.path.basename(self.gmsh_file)
        name, _ = os.path.splitext(file)

        # 输出节点文件
        nodes_file = os.path.join(output_dir, name + ".nodes")
        with open(nodes_file, "w") as f:
            for node in self.read_nodes():
                f.write("{:.6f} {:.6f} {:.6f}\n".format(*node))

        # 输出面文件
        faces_file = os.path.join(output_dir, name + ".faces")
        with open(faces_file, "w") as f:
            for face in self.read_faces():
                f.write("{:d} {:d} {:d}\n".format(*face))

        # 输出单元文件
        cells_file = os.path.join(output_dir, name + ".cells")
        with open(cells_file, "w") as f:
            for cell in self.read_cells():
                f.write("{:d} {:d} {:d} {:d}\n".format(*cell))


class ToGmsh:
    """
    将节点、面、单元信息导出到 Gmsh4 的 MESH 格式文件中。
    """

    def __init__(self, nodes, faces, cells):
        self.nodes = nodes
        self.faces = faces
        self.cells = cells

    def to_gmsh(self, output_dir: str = "."):
        """
        导出到 Gmsh4 的 MESH 格式文件中。

        Parameters:
            output_dir: 输出目录。
        """
        pass
