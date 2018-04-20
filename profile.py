# Import the Portal object.
import geni.portal as portal
# Import the ProtoGENI library.
import geni.rspec.pg as pg

# Create a portal context.
pc = portal.Context()

# Create a Request object to start building the RSpec.
request = pc.makeRequestRSpec()
 
# Add a raw PC to the request.
node = request.RawPC("node")

# Install and execute installlibs.sh
node.addService(pg.Execute(shell="sh", command="/local/repository/installibs.sh"))
# Install and execute installlibs.sh
node.addService(pg.Execute(shell="sh", command="/local/repository/instaltools.sh"))

# Print the RSpec to the enclosing page.
pc.printRequestRSpec(request)